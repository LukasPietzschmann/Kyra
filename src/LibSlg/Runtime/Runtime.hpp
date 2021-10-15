#pragma once

#include "../Exceptions.hpp"
#include "../Expressions/AccessExpr.hpp"
#include "../Expressions/AssignmentExpr.hpp"
#include "../Expressions/BinaryExpr.hpp"
#include "../Expressions/CallExpr.hpp"
#include "../Expressions/Expression.hpp"
#include "../Expressions/FunctionExpr.hpp"
#include "../Expressions/GroupExpr.hpp"
#include "../Expressions/InstantiationExpr.hpp"
#include "../Expressions/LiteralExpr.hpp"
#include "../Expressions/TypeExpr.hpp"
#include "../Expressions/UnaryExpr.hpp"
#include "../Expressions/VariableExpr.hpp"
#include "../Statements/BlockStmt.hpp"
#include "../Statements/ClassDeclarationStmt.hpp"
#include "../Statements/DeclarationStmt.hpp"
#include "../Statements/ExpressionStmt.hpp"
#include "../Statements/PrintStmt.hpp"
#include "../Statements/ReturnStmt.hpp"
#include "../Statements/Statement.hpp"
#include "../Values/Bool.hpp"
#include "../Values/Function.hpp"
#include "../Values/Klass.hpp"
#include "../Values/Number.hpp"
#include "RuntimeContext.hpp"

namespace Slanguage {
class Runtime : public ExpressionVisitor, public StatementVisitor {
	EXPR_NEEDS_VISIT_RETURN_OF_TYPE(Value::Ptr);

public:
	static Runtime& getInstance();

	Runtime(Runtime const&) = delete;
	Runtime(Runtime&&) noexcept = delete;
	Runtime& operator=(Runtime const&) = delete;
	Runtime& operator=(Runtime&&) = delete;

	// FIXME only used in Klass::instantiate. Looks like bad design
	Value::Ptr getVisitorReturn();

	void executeStatement(Statement::WeakPtr statement, RuntimeContext* contextToExecuteOn = nullptr);
	Value::Ptr executeExpression(Expression::WeakPtr expression);

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

	RuntimeContext m_currentContext{RuntimeContext()};

	template <typename Callback>
	RuntimeContext runInNewContext(const Callback& callback, RuntimeContext* parent);
};
}