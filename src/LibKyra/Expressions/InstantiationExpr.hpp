#pragma once

#include <string>
#include <vector>

#include "../Token.hpp"
#include "Expression.hpp"

namespace Kyra {
struct Position;
class InstantiationExpr : public Expression {
public:
	InstantiationExpr(const Position& position, Token name, std::vector<Expression::Ptr>&& arguments);
	~InstantiationExpr() override = default;

	void accept(ExpressionVisitor& visitor) override;
	const Token& get_class_name() const;
	const std::vector<Expression::Ptr>& get_arguments() const;

private:
	Token m_class_name;
	std::vector<Expression::Ptr> m_arguments;
};
}