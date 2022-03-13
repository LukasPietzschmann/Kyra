#pragma once

#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "../Expressions/LambdaFunctionExpr.hpp"
#include "../Runtime/RuntimeContext.hpp"
#include "Value.hpp"

namespace Kyra {
class Function : public Value {
public:
	Function(std::shared_ptr<LambdaFunctionExpr> function_expr, RuntimeContext::Ptr definition_context) :
		m_function_expr(std::move(function_expr)), m_definition_context(std::move(definition_context)) {}
	~Function() override = default;

	unsigned long get_arity() const { return m_function_expr->get_parameters().size(); }
	Value::Ptr exec(std::vector<Value::Ptr> arguments) const;

	bool is_implicitly_true() const override { return true; }
	std::string get_type() const override { return Value::NativeTypes::Function; }
	std::string to_string() const override { return "[Function]"; }

private:
	std::shared_ptr<LambdaFunctionExpr> m_function_expr;
	RuntimeContext::Ptr m_definition_context;
};
}