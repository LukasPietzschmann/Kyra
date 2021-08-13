#pragma once

#include "Expression.hpp"
#include "../Token.hpp"

namespace LibSlg {
class Variable : public Expression {
public:
	explicit Variable(const Token& name) : m_name(name) {}
private:
	Token m_name;
};
}