#pragma once

#include "Expression.hpp"
#include "../Statements/Statement.hpp"

namespace LibSlg {
class Object : public Expression {
public:
	explicit Object(Statement::Ptr impl) : m_implementation(std::move(impl)) {}
private:
	Statement::Ptr m_implementation;
};
}