#pragma once

#include <iostream>

#include "Expressions/Expression.hpp"
#include "Expressions/Forward.hpp"
#include "Statements/Forward.hpp"
#include "Statements/Statement.hpp"

#define COUT std::cout << std::string(m_indent * 4, ' ')

namespace Kyra {
class AstLogger : public StatementVisitor, public ExpressionVisitor {
public:
	AstLogger() = default;
	~AstLogger() override = default;

	void logStatement(const Statement::Ptr& statement);
	void visitBlockStmt(BlockStmt& blockStmt) override;
	void visitDeclarationStmt(DeclarationStmt& declarationStmt) override;
	void visitClassDeclarationStmt(ClassDeclarationStmt& classDeclarationStmt) override;
	void visitExpressionStmt(ExpressionStmt& expressionStmt) override;
	void visitPrintStmt(PrintStmt& printStmt) override;
	void visitReturnStmt(ReturnStmt& returnStmt) override;
	void visitWhileStmt(WhileStmt& whileStmt) override;
	void visitAccessExpr(AccessExpr& accessExpr) override;
	void visitAssignmentExpr(AssignmentExpr& assignmentExpr) override;
	void visitBinaryExpr(BinaryExpr& binaryExpr) override;
	void visitCallExpr(CallExpr& callExpr) override;
	void visitFunction(FunctionExpr& functionExpr) override;
	void visitGroupExpr(GroupExpr& groupExpr) override;
	void visitInstantiationExpr(InstantiationExpr& instantiationExpr) override;
	void visitLiteral(LiteralExpr& literal) override;
	void visitTypeExpr(TypeExpr& typeExpr) override;
	void visitUnaryExpr(UnaryExpr& unaryExpr) override;
	void visitVariable(VariableExpr& variableExpr) override;

private:
	unsigned int m_indent{};
};
}