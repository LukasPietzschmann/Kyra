#pragma once

#include "../Expressions/Expression.hpp"
#include "../HasPtrAlias.hpp"
#include "../Statements/Statement.hpp"
#include "../Values/Value.hpp"
#include "RuntimeContext.hpp"

namespace Kyra {
class AccessExpr;
class AssignmentExpr;
class BinaryExpr;
class BlockStmt;
class CallExpr;
class ClassDeclarationStmt;
class DeclarationStmt;
class ExpressionStmt;
class FunctionExpr;
class GroupExpr;
class InstantiationExpr;
class LiteralExpr;
class PrintStmt;
class ReturnStmt;
class TypeExpr;
class UnaryExpr;
class VariableExpr;

class Runtime : public ExpressionVisitor, public StatementVisitor {
	EXPR_NEEDS_VISIT_RETURN_OF_TYPE(Value::Ptr);

public:
	static Runtime& getInstance();

	Runtime(Runtime const&) = delete;
	Runtime(Runtime&&) noexcept = delete;
	Runtime& operator=(Runtime const&) = delete;
	Runtime& operator=(Runtime&&) = delete;

	void executeStatement(Statement::Ptr statement, RuntimeContext::Ptr contextToExecuteOn = nullptr);
	Value::Ptr executeExpression(Expression::Ptr expression, RuntimeContext::Ptr contextToExecuteOn = nullptr);

	void visitAccessExpr(AccessExpr& accessExpr) override;
	void visitAssignmentExpr(AssignmentExpr& assignmentExpr) override;
	void visitBinaryExpr(BinaryExpr& binaryExpr) override;
	void visitCallExpr(CallExpr& callExpr) override;
	void visitFunction(FunctionExpr& functionExpr) override;
	void visitGroupExpr(GroupExpr& groupExpr) override;
	void visitInstantiationExpr(InstantiationExpr& instantiationExpr) override;
	void visitLiteral(LiteralExpr& literalExpr) override;
	void visitTypeExpr(TypeExpr& typeExpr) override;
	void visitUnaryExpr(UnaryExpr& unaryExpr) override;
	void visitVariable(VariableExpr& variableExpr) override;

	void visitBlockStmt(BlockStmt& blockStmt) override;
	void visitDeclarationStmt(DeclarationStmt& declarationStmt) override;
	void visitClassDeclarationStmt(ClassDeclarationStmt& classDeclarationStmt) override;
	void visitExpressionStmt(ExpressionStmt& expressionStmt) override;
	void visitPrintStmt(PrintStmt& printStmt) override;
	void visitReturnStmt(ReturnStmt& returnStmt) override;

private:
	Runtime() = default;

	RuntimeContext::Ptr m_currentContext{RuntimeContext::makePtr<RuntimeContext>()};

	template <typename Callback>
	RuntimeContext::Ptr runInNewContext(const Callback& callback, RuntimeContext::Ptr parent);
};
}