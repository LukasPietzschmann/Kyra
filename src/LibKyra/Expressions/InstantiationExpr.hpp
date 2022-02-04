#pragma once

#include <string>
#include <vector>

#include "Expression.hpp"

namespace Kyra {
struct Position;
class InstantiationExpr : public Expression {
public:
	InstantiationExpr(const Position& position, std::string name, std::vector<Expression::Ptr> arguments);
	~InstantiationExpr() override = default;

	void accept(ExpressionVisitor& visitor) override;
	const std::string& getName() const;
	const std::vector<Expression::Ptr>& getArguments() const;

private:
	std::string m_name;
	std::vector<Expression::Ptr> m_arguments;
};
}