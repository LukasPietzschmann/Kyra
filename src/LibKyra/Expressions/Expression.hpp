#pragma once

#include "../HasPtrAlias.hpp"
#include "../Position.hpp"
#include "../Values/Value.hpp"
#include "Forward.hpp"

namespace Kyra {

class ExpressionVisitor {
public:
#define EXPR_NEEDS_VISIT_RETURN_OF_TYPE(type) \
private:                                      \
	type m_exprVisitorResult
#define EXPR_RETURN_FROM_VISIT(value) \
	do {                              \
		m_exprVisitorResult = value;  \
		return;                       \
	} while(0)
#define EXPR_GET_FROM_VISIT(dest) dest = m_exprVisitorResult
#define EXPR_ACCEPT(visitee, visitor, dest) \
	visitee->accept(visitor);               \
	EXPR_GET_FROM_VISIT(dest)

	virtual ~ExpressionVisitor() = default;

	virtual void visitAccessExpr(AccessExpr& accessExpr) = 0;
	virtual void visitAssignmentExpr(AssignmentExpr& assignmentExpr) = 0;
	virtual void visitBinaryExpr(BinaryExpr& binaryExpr) = 0;
	virtual void visitCallExpr(CallExpr& callExpr) = 0;
	virtual void visitFunction(FunctionExpr& functionExpr) = 0;
	virtual void visitGroupExpr(GroupExpr& groupExpr) = 0;
	virtual void visitInstantiationExpr(InstantiationExpr& instantiationExpr) = 0;
	virtual void visitLiteral(LiteralExpr& literalExpr) = 0;
	virtual void visitTypeExpr(TypeExpr& typeExpr) = 0;
	virtual void visitUnaryExpr(UnaryExpr& unaryExpr) = 0;
	virtual void visitVariable(VariableExpr& variableExpr) = 0;
};

class Expression : public HasPtrAlias<Expression> {
public:
	explicit Expression(const Position& position) : m_position(position) {}
	virtual ~Expression() = default;

	virtual void accept(ExpressionVisitor& visitor) = 0;

	const Position& getPosition() const { return m_position; }

protected:
	Position m_position;
};
}