#pragma once

#include <utility>
#include "Expression.hpp"
#include "../Token.hpp"

namespace LibSlg {
class Variable : public Expression {
public:
	explicit Variable(Token name) : m_name(std::move(name)) {}
	~Variable() override {}
	Value::Ptr accept(ExpressionVisitor& visitor) override { return visitor.visitVariable(*this); }
	const Token& getName() const { return m_name; }
private:
	Token m_name;
};
}