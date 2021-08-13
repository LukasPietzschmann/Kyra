#pragma once

#include <string>
#include "Statement.hpp"

namespace LibSlg {
class DeclarationStmt : public Statement {
public:
	DeclarationStmt(std::string identifier, Expression::Ptr initializer) :
			m_identifier(std::move(identifier)), m_initializer(std::move(initializer)) {}
	void accept(StatementVisitor& visitor) override { visitor.visitDeclarationStmt(*this); }
private:
	std::string m_identifier;
	Expression::Ptr m_initializer;
};
}