#pragma once

#include "../Token.hpp"
#include "Expression.hpp"

namespace Kyra {
struct Position;
class VariableExpr : public Expression {
public:
	VariableExpr(const Position& position, Token name);
	~VariableExpr() override = default;

	void accept(ExpressionVisitor& visitor) override;
	const Token& getName() const;

private:
	Token m_name;
};
}