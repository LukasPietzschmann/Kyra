#pragma once

#include "Expression.hpp"

namespace LibSlg {
class CallExpr : public Expression {
public:
	void accept(ExpressionVisitor& visitor) override { visitor.visitCallExpr(*this); }
};
}