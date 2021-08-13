#pragma once

#include "Expression.hpp"

namespace LibSlg {
template <typename T>
class Literal : public Expression {
public:
	explicit Literal(T value, bool isNothing = false) : m_value(value), m_isNothing(isNothing) {}
private:
	T m_value;
	bool m_isNothing;
};
}