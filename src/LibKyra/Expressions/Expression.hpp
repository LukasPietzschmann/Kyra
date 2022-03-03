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

	virtual void visit_access_expr(AccessExpr& access_expr) = 0;
	virtual void visit_assignment_expr(AssignmentExpr& assignment_expr) = 0;
	virtual void visit_binary_expr(BinaryExpr& binary_expr) = 0;
	virtual void visit_call_expr(CallExpr& call_expr) = 0;
	virtual void visit_function(FunctionExpr& function_expr) = 0;
	virtual void visit_group_expr(GroupExpr& group_expr) = 0;
	virtual void visit_instantiation_expr(InstantiationExpr& instantiation_expr) = 0;
	virtual void visit_literal(LiteralExpr& literal_expr) = 0;
	virtual void visit_type_expr(TypeExpr& type_expr) = 0;
	virtual void visit_unary_expr(UnaryExpr& unary_expr) = 0;
	virtual void visit_variable(VariableExpr& variable_expr) = 0;
};

class Expression : public HasPtrAlias<Expression> {
public:
	explicit Expression(const Position& position) : m_position(position) {}
	virtual ~Expression() = default;

	virtual void accept(ExpressionVisitor& visitor) = 0;

	const Position& get_position() const { return m_position; }

protected:
	Position m_position;
};
}