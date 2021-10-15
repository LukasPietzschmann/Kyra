#pragma once

#include <utility>

#include "../Token.hpp"
#include "Expression.hpp"

namespace Slanguage {
class VariableExpr : public Expression {
public:
	VariableExpr(const Position& position, Token name) : Expression(position), m_name(std::move(name)) {}
	~VariableExpr() override = default;
	void accept(ExpressionVisitor& visitor) override { return visitor.visitVariable(*this); }
	const Token& getName() const { return m_name; }

private:
	Token m_name;
};
}