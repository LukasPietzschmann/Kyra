#include "Parser.hpp"

namespace LibSlg {
std::vector<Statement::Ptr> Parser::parse() {
	while(!isAtEnd())
		m_statements.push_back(declaration());
	return m_statements;
}

Statement::Ptr Parser::declaration() {
	if(!matchAndAdvance(TokenType::VAR))
		return statement();

	std::string identifier = consume(TokenType::NAME).getValue().asString();
	Expression::Ptr init = nullptr;
	if(matchAndAdvance(TokenType::EQUAL))
		init = assignment();
	consume(TokenType::SEMICOLON);

	return Statement::makePtr<DeclarationStmt>(identifier, init);
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
Expression::Ptr Parser::primary() {
	if(matchAndAdvance(TokenType::LEFT_PAREN)) {
		Expression::Ptr expr = assignment();
		consume(TokenType::RIGHT_PAREN);
	}
	if(matchAndAdvance(TokenType::NOTHING))
		return Expression::makePtr<Literal>(Value());
	if(matchAndAdvance(TokenType::NUMBER))
		return Expression::makePtr<Literal>(Value(previous().getValue().asInt()));
	if(matchAndAdvance(TokenType::STRING))
		return Expression::makePtr<Literal>(Value(previous().getValue().asString()));
	if(matchAndAdvance(TokenType::TRUE))
		return Expression::makePtr<Literal>(Value(true));
	if(matchAndAdvance(TokenType::FALSE))
		return Expression::makePtr<Literal>(Value(false));
	if(matchAndAdvance(TokenType::NAME))
		return Expression::makePtr<Variable>(previous());
	if(match(TokenType::FUN))
		return function();
	if(match(TokenType::OBJECT))
		return object();

	std::cerr << "Expected Expression - Should not reach this code" << std::endl;
	return {};
}

Expression::Ptr Parser::function() {
	consume(TokenType::FUN);

	std::vector<Variable> parameters;
	consume(TokenType::LEFT_PAREN);
	if(!match(TokenType::RIGHT_PAREN)) {
		do {
			parameters.emplace_back(consume(TokenType::NAME));
		}while(matchAndAdvance(TokenType::COMMA));
	}
	consume(TokenType::RIGHT_PAREN);

	Statement::Ptr implementation = block();

	return Expression::makePtr<Function>(parameters, implementation);
}

Expression::Ptr Parser::object() {
	consume(TokenType::OBJECT);
	return Expression::makePtr<Object>(block());
}

Token Parser::advance() {
	return m_tokens[m_current++];
}

Token Parser::consume(TokenType expected) {
	if(match(expected))
		return advance();
	std::cerr << "[Error] expected " << TokenTypeName::getFor(expected) << " but got "
			<< TokenTypeName::getFor(peek().getType()) << std::endl;
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