#pragma once

#include <string>

#include "Expression.hpp"

namespace LibSlg {
class TypeExpr : public Expression {
public:
	explicit TypeExpr(std::string name) : m_name(std::move(name)) {}
	~TypeExpr() override = default;
	void accept(ExpressionVisitor& visitor) override { visitor.visitTypeExpr(*this); }

	const std::string& getName() const { return m_name; }

private:
	std::string m_name;
};
}