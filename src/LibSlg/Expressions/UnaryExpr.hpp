#pragma once

#include "Expression.hpp"
#include "../Token.hpp"

namespace LibSlg {
class UnaryExpr : public Expression {
public:
	UnaryExpr(const Token& oper, Expression::Ptr rhs) : m_operator(oper), m_rhs(std::move(rhs)) {}
private:
	Token m_operator;
	Expression::Ptr m_rhs;
};
}