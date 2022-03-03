#pragma once

#include "../Values/Value.hpp"
#include "Expression.hpp"

namespace Kyra {
struct Position;
class LiteralExpr : public Expression {
public:
	LiteralExpr(const Position& position, const Value::Ptr& value);
	~LiteralExpr() override = default;

	void accept(ExpressionVisitor& visitor) override;
	Value::Ptr get_value() const;

private:
	Value::Ptr m_value;
};
}