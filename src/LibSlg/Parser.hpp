#pragma once

#include <vector>
#include <iostream>
#include "Expressions/BinaryExpr.hpp"
#include "Expressions/Expression.hpp"
#include "Expressions/Function.hpp"
#include "Expressions/Literal.hpp"
#include "Expressions/Object.hpp"
#include "Expressions/UnaryExpr.hpp"
#include "Expressions/Variable.hpp"
#include "Statements/BlockStmt.hpp"
#include "Statements/DeclarationStmt.hpp"
#include "Statements/ExpressionStmt.hpp"
#include "Statements/PrintStmt.hpp"
#include "Statements/ReturnStmt.hpp"
#include "Statements/Statement.hpp"
#include "Token.hpp"

namespace LibSlg {
class Parser {
public:
	explicit Parser(const std::vector<Token>& tokens) : m_tokens(tokens) {
		m_statements.reserve(m_tokens.size());
	}

	std::vector<Statement::Ptr> parse();
private:
	const std::vector<Token> m_tokens;
	std::vector<Statement::Ptr> m_statements;
	int m_current {};

	Statement::Ptr declaration();
	Statement::Ptr statement();
	Statement::Ptr expression();
	Statement::Ptr block();
	Statement::Ptr print();
	Statement::Ptr ret();

	Expression::Ptr assignment();
	Expression::Ptr equality();
	Expression::Ptr comparison();
	Expression::Ptr term();
	Expression::Ptr factor();
	Expression::Ptr unary();
	Expression::Ptr call();
	Expression::Ptr primary();

	Token advance();
	Token consume(TokenType expected);
	Token peek() const;
	bool match(TokenType expected) const;
	bool matchAndAdvance(TokenType expected);
	bool match(std::initializer_list<TokenType> expected) const;
	bool matchAndAdvance(std::initializer_list<TokenType> expected);
	Token previous() const;
	bool isAtEnd() const;
};
}