#pragma once

#include "Expression.hpp"
#include "../Token.hpp"

namespace LibSlg {
class Variable : public Expression {
public:
	explicit Variable(const Token& name) : m_name(name) {}
	void accept(ExpressionVisitor& visitor) override { visitor.visitVariable(*this); }
	const Token& getName() const { return m_name; }
private:
	Token m_name;
};
}