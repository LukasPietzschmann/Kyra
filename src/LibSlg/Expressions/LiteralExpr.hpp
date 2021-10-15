#pragma once

#include "../Values/Value.hpp"
#include "Expression.hpp"

namespace Slanguage {
class LiteralExpr : public Expression {
public:
	LiteralExpr(const Position& position, Value::Ptr value) : Expression(position), m_value(std::move(value)) {}
	~LiteralExpr() override = default;
	void accept(ExpressionVisitor& visitor) override { return visitor.visitLiteral(*this); }
	const Value::Ptr& getValue() const { return m_value; }

private:
	Value::Ptr m_value;
};
}