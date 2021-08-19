#pragma once

#include "Expression.hpp"
#include "../Value.hpp"

namespace LibSlg {
class Literal : public Expression {
public:
	explicit Literal(const Value& value) : m_value(value) {}
	~Literal() override {}
	Value::Ptr accept(ExpressionVisitor& visitor) override { return visitor.visitLiteral(*this); }
	const Value& getValue() const { return m_value; }
private:
	Value m_value;
};
}