#pragma once

#include <utility>
#include <vector>
#include <iostream>
#include "Exceptions.hpp"
#include "Expressions/AccessExpr.hpp"
#include "Expressions/AssignmentExpr.hpp"
#include "Expressions/BinaryExpr.hpp"
#include "Expressions/CallExpr.hpp"
#include "Expressions/Expression.hpp"
#include "Expressions/FunctionExpr.hpp"
#include "Expressions/GroupExpr.hpp"
#include "Expressions/InstantiationExpr.hpp"
#include "Expressions/LiteralExpr.hpp"
#include "Expressions/UnaryExpr.hpp"
#include "Expressions/VariableExpr.hpp"
#include "Statements/BlockStmt.hpp"
#include "Statements/ClassDeclarationStmt.hpp"
#include "Statements/DeclarationStmt.hpp"
#include "Statements/ExpressionStmt.hpp"
#include "Statements/PrintStmt.hpp"
#include "Statements/ReturnStmt.hpp"
#include "Statements/Statement.hpp"
#include "Token.hpp"
#include "Values/Bool.hpp"
#include "Values/Function.hpp"
#include "Values/Nothing.hpp"
#include "Values/Number.hpp"
#include "Values/String.hpp"

namespace LibSlg {
class Parser {
public:
	explicit Parser(std::vector<Token> tokens) : m_tokens(std::move(tokens)) {
		m_statements.reserve(m_tokens.size());
	}

	std::vector<Statement::Ptr> parse();
private:
	const std::vector<Token> m_tokens;
	std::vector<Statement::Ptr> m_statements;
	int m_current {};

	Statement::Ptr declaration();
	Statement::Ptr varDeclaration();
	Statement::Ptr classDeclaration();
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
	Expression::Ptr function();
	Expression::Ptr instantiation();

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