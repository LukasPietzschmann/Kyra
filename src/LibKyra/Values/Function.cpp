#include "Function.hpp"

#include <cassert>

#include "../Runtime/Runtime.hpp"
#include "Nothing.hpp"

namespace Kyra {
Value::Ptr Function::exec(std::vector<Value::Ptr> arguments) const {
	assert(arguments.size() == get_arity());
	RuntimeContext::Ptr runtime_context = RuntimeContext::make_ptr<RuntimeContext>(m_definition_context);
	for(unsigned long i = 0; i < arguments.size(); ++i) {
		const auto& name = m_function_expr->get_parameters()[i].name.get_value().as_string();
		if(!runtime_context->mutate_var(name, arguments[i]) && !runtime_context->declare_var(name, arguments[i]))
			assert(false);
	}
	try {
		Runtime::the().execute_statement(m_function_expr->get_implementation(), runtime_context);
	} catch(const ReturnException& exception) { return exception.get_return_val(); }
	return Value::make_ptr<Nothing>();
}
}