#include "Parser.hpp"

namespace LibSlg {
std::vector<Statement> Parser::parse() {
	while(!isAtEnd())
		m_statements.push_back(*declaration());

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

	return std::make_shared<DeclarationStmt>(identifier, init);
}

Statement::Ptr Parser::statement() {
	if(match(TokenType::LEFT_CURLY))
		return std::make_shared<BlockStmt>(block());
	if(match(TokenType::PRINT))
		return print();
	if(match(TokenType::RETURN))
		return ret();
	return expression();
}

std::vector<Statement::Ptr> Parser::block() {
	consume(TokenType::LEFT_CURLY);

	std::vector<Statement::Ptr> statements;
	while(!match(TokenType::RIGHT_CURLY) && !isAtEnd())
		statements.push_back(declaration());

	consume(TokenType::RIGHT_CURLY);

	return statements;
}

Statement::Ptr Parser::print() {
	consume(TokenType::PRINT);
	Expression::Ptr expr = assignment();
	consume(TokenType::SEMICOLON);

	return std::make_shared<PrintStmt>(expr);
}

Statement::Ptr Parser::ret() {
	consume(TokenType::RETURN);
	Expression::Ptr expr = assignment();
	consume(TokenType::SEMICOLON);

	return std::make_shared<ReturnStmt>(expr);
}

Statement::Ptr Parser::expression() {
	Expression::Ptr expr = assignment();
	consume(TokenType::SEMICOLON);

	return std::make_shared<ExpressionStmt>(expr);
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

bool Parser::match(TokenType expected) const {
	if(isAtEnd())
		return false;
	return peek().getType() == expected;
}

bool Parser::matchAndAdvance(TokenType expected) {
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