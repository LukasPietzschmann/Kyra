#pragma once

#include "Expression.hpp"
#include "../Token.hpp"

namespace LibSlg {
class Variable : public Expression {
public:
	explicit Variable(const Token& name) : m_name(name) {}
	Value::Ptr accept(ExpressionVisitor& visitor) override { return visitor.visitVariable(*this); }
	const Token& getName() const { return m_name; }
private:
	Token m_name;
};
}