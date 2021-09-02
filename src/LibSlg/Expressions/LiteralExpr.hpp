#pragma once

#include "../Values/Value.hpp"
#include "Expression.hpp"

namespace LibSlg {
class LiteralExpr : public Expression {
public:
	explicit LiteralExpr(Value::Ptr value) : m_value(std::move(value)) {}
	~LiteralExpr() override {}
	void accept(ExpressionVisitor& visitor) override { return visitor.visitLiteral(*this); }
	const Value::Ptr& getValue() const { return m_value; }

private:
	Value::Ptr m_value;
};
}