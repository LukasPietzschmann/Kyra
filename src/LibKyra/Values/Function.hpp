#pragma once

#include <iosfwd>
#include <string>
#include <type_traits>
#include <vector>

#include "../Expressions/FunctionExpr.hpp"
#include "../HasPtrAlias.hpp"
#include "../Runtime/RuntimeContext.hpp"
#include "Value.hpp"

namespace Kyra {
class Function : public Value {
public:
	Function(const FunctionExpr& functionExpr, RuntimeContext::Ptr definitionContext) :
		m_functionExpr(functionExpr), m_definitionContext(definitionContext) {}
	~Function() override = default;

	unsigned long getArity() const { return m_functionExpr.getParameters().size(); }
	Value::Ptr exec(std::vector<Value::Ptr> arguments) const;

	bool isImplicitlyTrue() const override { return true; }
	std::string getType() const override { return Value::NativeTypes::Function; }
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
	RuntimeContext::Ptr m_definitionContext;
};
}