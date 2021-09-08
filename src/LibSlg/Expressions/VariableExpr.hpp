#pragma once

#include <utility>

#include "../Token.hpp"
#include "Expression.hpp"

namespace LibSlg {
class VariableExpr : public Expression {
public:
	explicit VariableExpr(Token name) : m_name(std::move(name)) {}
	~VariableExpr() override = default;
	void accept(ExpressionVisitor& visitor) override { return visitor.visitVariable(*this); }
	const Token& getName() const { return m_name; }

private:
	Token m_name;
};
}