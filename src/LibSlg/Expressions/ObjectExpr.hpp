#pragma once

#include "Expression.hpp"
#include "../Statements/Statement.hpp"

namespace LibSlg {
class ObjectExpr : public Expression {
public:
	explicit ObjectExpr(Statement::Ptr impl) : m_implementation(std::move(impl)) {}
	~ObjectExpr() override {}
	Value::Ptr accept(ExpressionVisitor& visitor) override { return visitor.visitObject(*this); }
	const Statement::Ptr& getImplementation() const { return m_implementation; }
private:
	Statement::Ptr m_implementation;
};
}