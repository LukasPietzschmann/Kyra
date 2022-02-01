#pragma once

#include <initializer_list>
#include <type_traits>
#include <vector>

#include "Expressions/Expression.hpp"
#include "HasPtrAlias.hpp"
#include "Statements/Statement.hpp"
#include "Token.hpp"
#include "TokenType.hpp"

namespace Kyra {
class Parser {
public:
	explicit Parser(std::vector<Token> tokens) : m_tokens(std::move(tokens)) { m_statements.reserve(m_tokens.size()); }

	std::vector<Statement::Ptr> parse();

private:
	const std::vector<Token> m_tokens;
	std::vector<Statement::Ptr> m_statements;
	int m_current{};

	Statement::Ptr declaration();
	Statement::Ptr varDeclaration();
	Statement::Ptr classDeclaration();
	Statement::Ptr statement();
	Statement::Ptr expression();
	Statement::Ptr block();
	Statement::Ptr print();
	Statement::Ptr ret();
	Statement::Ptr while_loop();

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

	Expression::Ptr typeIndicator();

	Token advance();
	Token consume(TokenType expected);
	Token consume(std::initializer_list<TokenType> expected);
	Token peek() const;
	bool match(TokenType expected) const;
	bool matchAndAdvance(TokenType expected);
	bool match(std::initializer_list<TokenType> expected) const;
	bool matchAndAdvance(std::initializer_list<TokenType> expected);
	Token previous() const;
	bool isAtEnd() const;
};
}