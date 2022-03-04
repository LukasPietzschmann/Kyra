#pragma once

#include "../Token.hpp"
#include "Expression.hpp"

namespace Kyra {
struct Position;
class AssignmentExpr : public Expression {
public:
	AssignmentExpr(const Position& position, Expression::Ptr owner, Token name, Expression::Ptr new_value);
	~AssignmentExpr() override = default;

	void accept(ExpressionVisitor& visitor) override;
	Expression::Ptr get_owner() const;
	const Token& get_name() const;
	Expression::Ptr get_new_value() const;

private:
	Expression::Ptr m_owner;
	Token m_name;
	Expression::Ptr m_new_value;
};
}