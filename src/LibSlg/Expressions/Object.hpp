#pragma once

#include "Expression.hpp"
#include "../Statements/Statement.hpp"

namespace LibSlg {
class Object : public Expression {
public:
	explicit Object(Statement::Ptr impl) : m_implementation(std::move(impl)) {}
	Value::Ptr accept(ExpressionVisitor& visitor) override { return visitor.visitObject(*this); }
	const Statement::Ptr& getImplementation() const { return m_implementation; }
private:
	Statement::Ptr m_implementation;
};
}