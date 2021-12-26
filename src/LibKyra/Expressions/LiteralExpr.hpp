#pragma once

#include "../Values/Value.hpp"
#include "Expression.hpp"

namespace Kyra {
class LiteralExpr : public Expression {
public:
	LiteralExpr(const Position& position, const Value::Ptr& value) : Expression(position), m_value(value) {}
	~LiteralExpr() override = default;
	void accept(ExpressionVisitor& visitor) override { return visitor.visitLiteral(*this); }
	Value::Ptr getValue() const { return m_value; }

private:
	Value::Ptr m_value;
};
}