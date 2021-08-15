#pragma once

#include "Expression.hpp"
#include "../Token.hpp"

namespace LibSlg {
class AssignmentExpr : public Expression {
public:
	const Token& getName() const { return m_name; }
	const Expression::Ptr& getNewValue() const { return m_newValue; }
private:
	Token m_name;
	Expression::Ptr m_newValue;
};
}