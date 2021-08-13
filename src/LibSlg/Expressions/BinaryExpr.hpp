#pragma once

#include "Expression.hpp"
#include "../Token.hpp"

namespace LibSlg {
class BinaryExpr : public Expression {
public:
	BinaryExpr(Expression::Ptr  lhs, const Token& oper, Expression::Ptr  rhs) :
			m_lhs(std::move(lhs)), m_operator(oper), m_rhs(std::move(rhs)) {}
private:
	Token m_operator;
	Expression::Ptr m_lhs;
	Expression::Ptr m_rhs;
};
}