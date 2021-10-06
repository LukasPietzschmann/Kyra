#pragma once

#include <utility>

#include "../Context.hpp"
#include "../Expressions/FunctionExpr.hpp"
#include "Nothing.hpp"
#include "Value.hpp"

namespace LibSlg {
class Function : public Value {
public:
	Function(const FunctionExpr& functionExpr, Context definitionContext) :
		m_functionExpr(functionExpr), m_definitionContext(std::move(definitionContext)) {}
	~Function() override = default;

	unsigned long getArity() const { return m_functionExpr.getParameters().size(); }
	Value::Ptr exec(std::vector<Value::Ptr> arguments) const;

	bool isImplicitlyTrue() const override { return true; }
	Type getType() const override { return Value::NativeTypes::Function; }
	std::string toString() const override { return "[Function]"; }

	bool operator==(const Value::Ptr& other) const override {
		if(getType() != other->getType())
			return false;
		return false;  // TODO better equality checking
	}

	bool operator<(const Value::Ptr&) const override { return false; }
	bool operator>(const Value::Ptr&) const override { return false; }

private:
	FunctionExpr m_functionExpr;
	Context m_definitionContext;
};
}