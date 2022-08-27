#include "Parser.hpp"

#include <memory>

#include "SourceRange.hpp"

const std::vector<RefPtr<Statement>>& Parser::parse_tokens(const std::vector<Token>& tokens) {
	m_statements.clear();
	m_tokens = &tokens;
	m_current_token = m_tokens->begin();

	while(!is_at_end())
		m_statements.push_back(declaration());

	return m_statements;
}

RefPtr<Statement> Parser::statement() {
	if(match(TokenType::LEFT_CURLY))
		return block();
	if(match(TokenType::RETURN))
		return return_statement();
	return expression_statement();
}

RefPtr<Statement> Parser::expression_statement() {
	RefPtr<Expression> expr = expression();
	const Token& semi_colon = consume(TokenType::SEMICOLON);
	return mk_ref<ExpressionStatement>(SourceRange::unite(expr->get_source_range(), semi_colon.get_source_range()),
			expr);
}

RefPtr<Statement> Parser::return_statement() {
	const Token& return_stmt = consume(TokenType::RETURN);
	RefPtr<Expression> expr = expression();
	const Token& semicolon = consume(TokenType::SEMICOLON);
	return mk_ref<Return>(SourceRange::unite(return_stmt.get_source_range(), semicolon.get_source_range()), expr);
}

RefPtr<Statement> Parser::block() {
	const Token& begin_curly = consume(TokenType::LEFT_CURLY);
	std::vector<RefPtr<Statement>> body;
	while(!match(TokenType::RIGHT_CURLY))
		body.push_back(declaration());
	const Token& end_curly = consume(TokenType::RIGHT_CURLY);
	return mk_ref<Block>(SourceRange::unite(begin_curly.get_source_range(), end_curly.get_source_range()), body);
}

RefPtr<Statement> Parser::declaration() {
	if(match(TokenType::VAL, TokenType::VAR))
		return variable_declaration();
	if(match(TokenType::FUN))
		return function_declaration();
	return statement();
}

RefPtr<Statement> Parser::variable_declaration() {
	const Token& val_var = consume(TokenType::VAL, TokenType::VAR);
	const Token& identifier = consume(TokenType::NAME);
	RefPtr<Type> var_type = std::static_pointer_cast<Type>(type());
	RefPtr<Expression> initializer;
	if(match_and_advance(TokenType::EQUAL))
		initializer = expression();
	const Token& semicolon = consume(TokenType::SEMICOLON);
	return mk_ref<Declaration>(SourceRange::unite(val_var.get_source_range(), semicolon.get_source_range()),
			val_var.get_type() == TokenType::VAL ? Declaration::Kind::VAL : Declaration::Kind::VAR,
			identifier.get_lexeme(),
			var_type,
			initializer);
}

RefPtr<Statement> Parser::function_declaration() {
	const Token& fun = consume(TokenType::FUN);
	const Token& identifier = consume(TokenType::NAME);
	consume(TokenType::LEFT_PAREN);
	std::vector<Function::Parameter> params;
	if(!match(TokenType::RIGHT_PAREN)) {
		do {
			const Token& val_var = consume(TokenType::VAL, TokenType::VAR);
			const Token& identifier = consume(TokenType::NAME);
			RefPtr<Type> param_type = std::static_pointer_cast<Type>(type());
			params.emplace_back(identifier.get_lexeme(),
					param_type,
					val_var.get_type() == TokenType::VAL ? Declaration::Kind::VAL : Declaration::Kind::VAR);

		} while(match_and_advance(TokenType::COMMA));
	}
	consume(TokenType::RIGHT_PAREN);
	RefPtr<Type> return_type = std::static_pointer_cast<Type>(type());
	RefPtr<Statement> implementation = block();
	return mk_ref<Function>(SourceRange::unite(fun.get_source_range(), implementation->get_source_range()),
			identifier.get_lexeme(),
			implementation,
			return_type,
			params);
}

RefPtr<Expression> Parser::expression() { return assignment(); }

RefPtr<Expression> Parser::assignment() {
	RefPtr<Expression> lhs = term();
	if(!match(TokenType::EQUAL))
		return lhs;
	const std::string_view identifier = std::static_pointer_cast<VarQuery>(lhs)->get_identifier();
	RefPtr<Expression> new_value = expression();
	return mk_ref<Assignment>(SourceRange::unite(lhs->get_source_range(), new_value->get_source_range()),
			identifier,
			new_value);
}

RefPtr<Expression> Parser::term() {
	RefPtr<Expression> lhs = factor();
	while(true) {
		const Token& oper = *m_current_token;
		if(!match_and_advance(TokenType::MINUS, TokenType::PLUS))
			break;
		RefPtr<Expression> rhs = factor();
		lhs = mk_ref<BinaryExpression>(SourceRange::unite(lhs->get_source_range(), rhs->get_source_range()),
				lhs,
				rhs,
				oper);
	}
	return lhs;
}

RefPtr<Expression> Parser::factor() {
	RefPtr<Expression> lhs = call();
	while(true) {
		const Token& oper = *m_current_token;
		if(!match_and_advance(TokenType::SLASH, TokenType::STAR))
			break;
		RefPtr<Expression> rhs = call();
		lhs = mk_ref<BinaryExpression>(SourceRange::unite(lhs->get_source_range(), rhs->get_source_range()),
				lhs,
				rhs,
				oper);
	}
	return lhs;
}

RefPtr<Expression> Parser::call() {
	RefPtr<Expression> lhs = primary();
	if(!match(TokenType::LEFT_PAREN))
		return lhs;
	consume(TokenType::LEFT_PAREN);
	std::vector<Call::Argument> args;
	if(!match(TokenType::RIGHT_PAREN)) {
		do {
			args.push_back(expression());
		} while(match_and_advance(TokenType::COMMA));
	}
	const Token& right_paren = consume(TokenType::RIGHT_PAREN);
	RefPtr<VarQuery> var_query = std::static_pointer_cast<VarQuery>(lhs);
	return mk_ref<Call>(SourceRange::unite(lhs->get_source_range(), right_paren.get_source_range()),
			var_query->get_identifier(),
			args);
}

RefPtr<Expression> Parser::primary() {
	if(match(TokenType::NUMBER)) {
		const Token& literal = consume(TokenType::NUMBER);
		return mk_ref<IntLiteral>(literal.get_source_range(), literal.get_literal_value().as_int());
	}
	if(match(TokenType::NAME)) {
		const Token& identifier = consume(TokenType::NAME);
		return mk_ref<VarQuery>(identifier.get_source_range(), identifier.get_literal_value().as_string());
	}
	// Group
	const Token& open_paren = consume(TokenType::LEFT_PAREN);
	RefPtr<Expression> content = expression();
	const Token& close_paren = consume(TokenType::RIGHT_PAREN);
	return mk_ref<Group>(SourceRange::unite(open_paren.get_source_range(), close_paren.get_source_range()), content);
}

RefPtr<Expression> Parser::type() {
	consume(TokenType::COLON);
	const Token& identifier = consume(TokenType::NAME);
	return mk_ref<Type>(identifier.get_source_range(), identifier.get_literal_value().as_string());
}

bool Parser::is_at_end() const { return m_current_token->get_type() == TokenType::END_OF_FILE; }
