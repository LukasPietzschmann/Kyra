#pragma once

#include <iostream>
#include <sstream>

#include "Expressions/AccessExpr.hpp"
#include "Expressions/AssignmentExpr.hpp"
#include "Expressions/BinaryExpr.hpp"
#include "Expressions/CallExpr.hpp"
#include "Expressions/Expression.hpp"
#include "Expressions/FunctionExpr.hpp"
#include "Expressions/GroupExpr.hpp"
#include "Expressions/InstantiationExpr.hpp"
#include "Expressions/LiteralExpr.hpp"
#include "Expressions/TypeExpr.hpp"
#include "Expressions/UnaryExpr.hpp"
#include "Expressions/VariableExpr.hpp"
#include "Statements/BlockStmt.hpp"
#include "Statements/ClassDeclarationStmt.hpp"
#include "Statements/DeclarationStmt.hpp"
#include "Statements/ExpressionStmt.hpp"
#include "Statements/PrintStmt.hpp"
#include "Statements/ReturnStmt.hpp"
#include "Statements/Statement.hpp"
#include "TokenType.hpp"

#define COUT std::cout << std::string(m_indent * 4, ' ')

namespace Kyra {
class AstLogger : public StatementVisitor, public ExpressionVisitor {
public:
	AstLogger() = default;
	~AstLogger() override = default;
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
		COUT << "Declaration of variable " << declarationStmt.getIdentifier().getValue().asString();
		if(declarationStmt.getInitializer()) {
			std::cout << " to " << std::endl;
			++m_indent;
			declarationStmt.getInitializer()->accept(*this);
			--m_indent;
		} else
			std::cout << std::endl;
	}

	void visitClassDeclarationStmt(ClassDeclarationStmt& classDeclarationStmt) override {
		COUT << "Declaration of class " << classDeclarationStmt.getIdentifier().getValue().asString() << "\n";
		COUT << "Constructor parameter\n";
		++m_indent;
		std::stringstream parameter;
		for(const auto& param : classDeclarationStmt.getConstructorParameters())
			parameter << param.name.getValue().asString() << " ";
		if(!classDeclarationStmt.getConstructorParameters().empty())
			COUT << parameter.str() << "\n";
		--m_indent;
		std::cout << "Declarations" << std::endl;
		++m_indent;
		for(const auto& decl : classDeclarationStmt.getDeclarations())
			decl->accept(*this);
		--m_indent;
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

	void visitFunction(FunctionExpr& functionExpr) override {
		std::stringstream parameters;
		for(const auto& parameter : functionExpr.getParameters())
			parameters << parameter.name.getValue().asString() << " ";
		COUT << "Function" << std::endl;
		COUT << "Parameter " << parameters.str() << std::endl;
		COUT << "Implementation" << std::endl;
		++m_indent;
		functionExpr.getImplementation()->accept(*this);
		--m_indent;
	}

	void visitGroupExpr(GroupExpr& groupExpr) override {
		COUT << "Group" << std::endl;
		++m_indent;
		groupExpr.getExpr()->accept(*this);
		--m_indent;
	}

	void visitInstantiationExpr(InstantiationExpr& instantiationExpr) override {
		COUT << "Instantiation of class " + instantiationExpr.getName() + "\n";
		COUT << "With constructor arguments\n";
		++m_indent;
		for(const auto& argument : instantiationExpr.getArguments())
			argument->accept(*this);
		--m_indent;
	}

	void visitLiteral(LiteralExpr& literal) override {
		COUT << "Literal with value of " << literal.getValue()->toString() << std::endl;
	}

	void visitTypeExpr(TypeExpr& typeExpr) override { COUT << "Type " << typeExpr.getName(); }

	void visitUnaryExpr(UnaryExpr& unaryExpr) override {
		COUT << "Unary Expr with Operand " << TokenTypeName::getFor(unaryExpr.getOperator().getType()) << std::endl;
		COUT << "Rhs: " << std::endl;
		++m_indent;
		unaryExpr.getRhs()->accept(*this);
		--m_indent;
	}

	void visitVariable(VariableExpr& variableExpr) override {
		COUT << "Variable with identifier " << variableExpr.getName().getValue().asString() << std::endl;
	}

private:
	unsigned int m_indent{};
};
}

#undef COUT