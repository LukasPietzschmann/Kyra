#pragma once

#include <iostream>
#include "Statements/Statement.hpp"

namespace LibSlg {
class AstLogger : public StatementVisitor {
public:
	void logStatement(const Statement::Ptr& statement) { statement->accept(*this); }

	void visitBlockStmt(BlockStmt& blockStmt) override {
		std::cout << "Block" << std::endl;
	}

	void visitDeclarationStmt(DeclarationStmt& declarationStmt) override {
		std::cout << "Declaration of variable " << declarationStmt.getIdentifier() << std::endl;
	}

	void visitExpressionStmt(ExpressionStmt& expressionStmt) override {
		std::cout << "ExprStmt" << std::endl;
	}

	void visitPrintStmt(PrintStmt& printStmt) override {
		std::cout << "Print" << std::endl;
	}

	void visitReturnStmt(ReturnStmt& returnStmt) override {
		std::cout << "Return" << std::endl;
	}
};
}