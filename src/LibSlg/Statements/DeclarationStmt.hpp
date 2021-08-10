#pragma once

#include <string>
#include "Statement.hpp"

class DeclarationStmt : public Statement {
public:
	DeclarationStmt(std::string identifier, Expression* initializer) :
		m_identifier(std::move(identifier)), m_initializer(initializer) {}
private:
	std::string m_identifier;
	Expression* m_initializer;
};