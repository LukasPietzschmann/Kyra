#pragma once

#include <sstream>
#include <iostream>
#include "TokenType.hpp"
#include "Statements/Statement.hpp"
#include "Expressions/Expression.hpp"
#include "Expressions/AccessExpr.hpp"   // FIXME There should be no need to include this explicitly.
										// Works for all other Expressions, but AccessExpr

#define COUT std::cout << std::string(m_indent * 4, ' ')

namespace LibSlg {
class AstLogger : public StatementVisitor, public ExpressionVisitor {
public:
	void logStatement(const Statement::Ptr& statement) {
		m_indent = 0;
		statement->accept(*this);
	}

	void visitBlockStmt(BlockStmt& blockStmt) override {
		COUT << "Block" << std::endl;
		++m_indent;
		for(const auto& stmt : blockStmt.getStatements())
			stmt->accept(*this);
		--m_indent;
	}

	void visitDeclarationStmt(DeclarationStmt& declarationStmt) override {
		COUT << "Declaration of variable " << declarationStmt.getIdentifier();
		if(declarationStmt.getInitializer()) {
			std::cout << " to " << std::endl;
			++m_indent;
			declarationStmt.getInitializer()->accept(*this);
			--m_indent;
		}else
			std::cout << std::endl;
	}

	void visitExpressionStmt(ExpressionStmt& expressionStmt) override {
		COUT << "Expression" << std::endl;
		++m_indent;
		expressionStmt.getExpr()->accept(*this);
		--m_indent;
	}

	void visitPrintStmt(PrintStmt& printStmt) override {
		COUT << "Print" << std::endl;
		++m_indent;
		printStmt.getExpr()->accept(*this);
		--m_indent;
	}

	void visitReturnStmt(ReturnStmt& returnStmt) override {
		COUT << "Return" << std::endl;
		++m_indent;
		returnStmt.getExpr()->accept(*this);
		--m_indent;
	}

	void visitAccessExpr(AccessExpr& accessExpr) override {
		COUT << "Get Variable " << accessExpr.getName().getValue().asString() << " from " << std::endl;
		++m_indent;
		accessExpr.getOwner()->accept(*this);
		--m_indent;
	}

	void visitAssignmentExpr(AssignmentExpr& assignmentExpr) override {
		COUT << "Assignment of variable " << assignmentExpr.getName().getValue().asString() << " to " << std::endl;
		++m_indent;
		assignmentExpr.getNewValue()->accept(*this);
		--m_indent;
	}

	void visitBinaryExpr(BinaryExpr& binaryExpr) override {
		COUT << "Binary Expr with Operand " << TokenTypeName::getFor(binaryExpr.getOperator().getType()) << std::endl;
		COUT << "Lhs: " << std::endl;
		++m_indent;
		binaryExpr.getLhs()->accept(*this);
		--m_indent;
		COUT << "Rhs: " << std::endl;
		++m_indent;
		binaryExpr.getRhs()->accept(*this);
		--m_indent;
	}

	void visitCallExpr(CallExpr& callExpr) override {
		COUT << "Call to" << std::endl;
		++m_indent;
		callExpr.getFunction()->accept(*this);
		--m_indent;
		COUT << "With arguments" << std::endl;
		++m_indent;
		for(const auto& argument : callExpr.getArguments())
			argument->accept(*this);
		--m_indent;
	}

	void visitFunction(Function& functionExpr) override {
		std::stringstream parameters;
		for(const auto& parameter : functionExpr.getParameters())
			parameters << parameter.getName().getValue().asString() << " ";
		COUT << "Function" << std::endl;
		COUT << "Parameter " << parameters.str() << std::endl;
		COUT << "Implementation" << std::endl;
		++m_indent;
		functionExpr.getImplementation()->accept(*this);
		--m_indent;
	}

	void visitLiteral(Literal& literal) override {
		COUT << "Literal with value of " << literal.getValue() << std::endl;
	}

	void visitObject(Object& objectExpr) override {
		COUT << "Object" << std::endl;
		COUT << "Implementation" << std::endl;
		++m_indent;
		objectExpr.getImplementation()->accept(*this);
		--m_indent;
	}

	void visitUnaryExpr(UnaryExpr& unaryExpr) override {
		COUT << "Unary Expr with Operand " << TokenTypeName::getFor(unaryExpr.getOperator().getType()) << std::endl;
		COUT << "Rhs: " << std::endl;
		++m_indent;
		unaryExpr.getRhs()->accept(*this);
		--m_indent;
	}

	void visitVariable(Variable& variableExpr) override {
		COUT << "Variable with identifier " << variableExpr.getName().getValue().asString() << std::endl;
	}
private:
	unsigned int m_indent {};
};
}

#undef COUT