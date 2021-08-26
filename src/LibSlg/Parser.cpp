#include "Parser.hpp"

namespace LibSlg {
std::vector<Statement::Ptr> Parser::parse() {
	while(!isAtEnd())
		m_statements.push_back(declaration());
	return m_statements;
}

Statement::Ptr Parser::declaration() {
	if(!matchAndAdvance({TokenType::VAR, TokenType::VAL}))
		return statement();

	bool isMutable = previous().getType() == TokenType::VAR;

	Token identifier = consume(TokenType::NAME);

	consume(TokenType::COLON);
	Value::Type type = consume(TokenType::NAME).getValue().asString();

	Expression::Ptr init = nullptr;
	if(matchAndAdvance(TokenType::EQUAL))
		init = assignment();
	consume(TokenType::SEMICOLON);

	return Statement::makePtr<DeclarationStmt>(identifier, init, type, isMutable);
}

Statement::Ptr Parser::statement() {
	if(match(TokenType::LEFT_CURLY))
		return block();
	if(match(TokenType::PRINT))
		return print();
	if(match(TokenType::RETURN))
		return ret();
	return expression();
}

Statement::Ptr Parser::block() {
	consume(TokenType::LEFT_CURLY);

	std::vector<Statement::Ptr> statements;
	while(!match(TokenType::RIGHT_CURLY) && !isAtEnd())
		statements.push_back(declaration());

	consume(TokenType::RIGHT_CURLY);

	return Statement::makePtr<BlockStmt>(statements);
}

Statement::Ptr Parser::print() {
	consume(TokenType::PRINT);
	Expression::Ptr expr = assignment();
	consume(TokenType::SEMICOLON);

	return Statement::makePtr<PrintStmt>(expr);
}

Statement::Ptr Parser::ret() {
	consume(TokenType::RETURN);
	Expression::Ptr expr = assignment();
	consume(TokenType::SEMICOLON);

	return Statement::makePtr<ReturnStmt>(expr);
}

Statement::Ptr Parser::expression() {
	Expression::Ptr expr = assignment();
	consume(TokenType::SEMICOLON);

	return Statement::makePtr<ExpressionStmt>(expr);
}

Expression::Ptr Parser::assignment() {
	Expression::Ptr expr = equality();

	if(matchAndAdvance(TokenType::EQUAL)) {
		auto accessExpression = std::dynamic_pointer_cast<AccessExpr>(expr);
		auto variableExpression = std::dynamic_pointer_cast<VariableExpr>(expr);
		if(accessExpression) {//FIXME: May god forgive me for using std::dynamic_pointer_cast
			Expression::Ptr newValue = assignment();
			expr = Expression::makePtr<AssignmentExpr>(accessExpression->getOwner(), accessExpression->getName(),
					newValue);
		}else if(variableExpression) {
			Expression::Ptr newValue = assignment();
			expr = Expression::makePtr<AssignmentExpr>(nullptr, variableExpression->getName(), newValue);
		}
	}

	return expr;
}

Expression::Ptr Parser::equality() {
	Expression::Ptr expr = comparison();

	while(true) {
		if(!matchAndAdvance({TokenType::BANG_EQUAL, TokenType::EQUAL_EQUAL}))
			break;
		Token oper = previous();
		Expression::Ptr rhs = comparison();
		expr = Expression::makePtr<BinaryExpr>(expr, oper, rhs);
	}

	return expr;
}

Expression::Ptr Parser::comparison() {
	Expression::Ptr expr = term();

	while(true) {
		if(!(matchAndAdvance({TokenType::GREATER, TokenType::GREATER_EQUAL, TokenType::LESS, TokenType::LESS_EQUAL})))
			break;
		Token oper = previous();
		Expression::Ptr rhs = term();
		expr = Expression::makePtr<BinaryExpr>(expr, oper, rhs);
	}

	return expr;
}

Expression::Ptr Parser::term() {
	Expression::Ptr expr = factor();

	while(true) {
		if(!(matchAndAdvance({TokenType::MINUS, TokenType::PLUS})))
			break;
		Token oper = previous();
		Expression::Ptr rhs = factor();
		expr = Expression::makePtr<BinaryExpr>(expr, oper, rhs);
	}

	return expr;
}

Expression::Ptr Parser::factor() {
	Expression::Ptr expr = unary();

	while(true) {
		if(!(matchAndAdvance({TokenType::SLASH, TokenType::STAR})))
			break;
		Token oper = previous();
		Expression::Ptr rhs = unary();
		expr = Expression::makePtr<BinaryExpr>(expr, oper, rhs);
	}

	return expr;
}

Expression::Ptr Parser::unary() {
	if(!(matchAndAdvance({TokenType::BANG, TokenType::MINUS})))
		return call();

	Token oper = previous();
	Expression::Ptr rhs = unary();
	return Expression::makePtr<UnaryExpr>(oper, rhs);
}

Expression::Ptr Parser::call() {
	Expression::Ptr expr = primary();

	while(true) {
		if(matchAndAdvance(TokenType::DOT)) {
			Token name = consume(TokenType::NAME);
			expr = Expression::makePtr<AccessExpr>(expr, name);
		}else if(matchAndAdvance(TokenType::LEFT_PAREN)) {
			bool needsComma = false;
			std::vector<Expression::Ptr> arguments;
			while(!matchAndAdvance(TokenType::RIGHT_PAREN)) {
				if(needsComma)
					consume(TokenType::COMMA);
				arguments.push_back(assignment());
				needsComma = true;
			}
			expr = Expression::makePtr<CallExpr>(expr, arguments);
		}else
			break;
	}

	return expr;
}

Expression::Ptr Parser::primary() {
	if(matchAndAdvance(TokenType::LEFT_PAREN)) {
		Expression::Ptr expr = assignment();
		consume(TokenType::RIGHT_PAREN);
		return Expression::makePtr<GroupExpr>(expr);
	}
	if(matchAndAdvance(TokenType::NOTHING))
		return Expression::makePtr<LiteralExpr>(Value::makePtr<Nothing>());
	if(matchAndAdvance(TokenType::NUMBER))
		return Expression::makePtr<LiteralExpr>(Value::makePtr<Number>(previous().getValue().asInt()));
	if(matchAndAdvance(TokenType::STRING))
		return Expression::makePtr<LiteralExpr>(Value::makePtr<String>(previous().getValue().asString()));
	if(matchAndAdvance(TokenType::TRUE))
		return Expression::makePtr<LiteralExpr>(Value::makePtr<Bool>(true));
	if(matchAndAdvance(TokenType::FALSE))
		return Expression::makePtr<LiteralExpr>(Value::makePtr<Bool>(false));
	if(matchAndAdvance(TokenType::NAME))
		return Expression::makePtr<VariableExpr>(previous());
	if(match(TokenType::FUN))
		return function();

	if(previous().getType() == TokenType::END_OF_FILE)
		throw ParserException(
				"Expected expression before " + peek().getLexeme() + " in line " + std::to_string(peek().getLine()));
	else
		throw ParserException("Expected expression after " + previous().getLexeme() +
				" in line " + std::to_string(previous().getLine()), true);
}

Expression::Ptr Parser::function() {
	consume(TokenType::FUN);

	std::vector<FunctionExpr::Parameter> parameters;
	consume(TokenType::LEFT_PAREN);
	if(!match(TokenType::RIGHT_PAREN)) {
		do {
			Token name = consume(TokenType::NAME);
			consume(TokenType::COLON);
			Value::Type type = consume(TokenType::NAME).getValue().asString();
			parameters.emplace_back(name, type);
		}while(matchAndAdvance(TokenType::COMMA));
	}
	consume(TokenType::RIGHT_PAREN);

	Statement::Ptr implementation = block();

	return Expression::makePtr<FunctionExpr>(parameters, implementation);
}

Token Parser::advance() {
	return m_tokens[m_current++];
}

Token Parser::consume(TokenType expected) {
	Token next = peek();
	if(!isAtEnd() && next.getType() == expected)
		return advance();
	bool unfinished = next.getType() == TokenType::END_OF_FILE;
	throw ParserException("Expected " + TokenTypeName::getFor(expected) + " but got " +
			TokenTypeName::getFor(peek().getType()) + " at line " + std::to_string(peek().getLine()), unfinished);
}

Token Parser::peek() const {
	return m_tokens[m_current];
}

bool Parser::match(TokenType expected) const { return match({expected}); }
bool Parser::matchAndAdvance(TokenType expected) { return matchAndAdvance({expected}); }

bool Parser::match(std::initializer_list<TokenType> expected) const {
	if(isAtEnd())
		return false;
	return std::any_of(expected.begin(), expected.end(), [this](TokenType type) {
		return type == peek().getType();
	});
}

bool Parser::matchAndAdvance(std::initializer_list<TokenType> expected) {
	bool res = match(expected);
	if(res)
		advance();
	return res;
}

Token Parser::previous() const {
	return m_tokens[m_current - 1];
}

bool Parser::isAtEnd() const {
	return peek().getType() == TokenType::END_OF_FILE;
}
}