#pragma once

#include <string>
#include <utility>
#include <vector>

#include "../Expressions/FunctionExpr.hpp"
#include "../Runtime/RuntimeContext.hpp"
#include "Value.hpp"

namespace Kyra {
class Function : public Value {
public:
	Function(const FunctionExpr& function_expr, RuntimeContext::Ptr definition_context) :
		m_function_expr(function_expr), m_definition_context(std::move(definition_context)) {}
	~Function() override = default;

	unsigned long get_arity() const { return m_function_expr.get_parameters().size(); }
	Value::Ptr exec(std::vector<Value::Ptr> arguments) const;

	bool is_implicitly_true() const override { return true; }
	std::string get_type() const override { return Value::NativeTypes::Function; }
	std::string to_string() const override { return "[Function]"; }

	bool operator==(const Value::Ptr& other) const override {
		if(get_type() != other->get_type())
			return false;
		return false;  // TODO better equality checking
	}

	bool operator<(const Value::Ptr&) const override { return false; }
	bool operator>(const Value::Ptr&) const override { return false; }

private:
	FunctionExpr m_function_expr;
	RuntimeContext::Ptr m_definition_context;
};
}