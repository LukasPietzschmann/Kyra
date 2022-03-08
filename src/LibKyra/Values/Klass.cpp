#include "Klass.hpp"

#include <cassert>
#include <memory>

#include "../Runtime/Runtime.hpp"
#include "../Statements/DeclarationStmt.hpp"
#include "Nothing.hpp"

namespace Kyra {
// TODO: instantiate should return a new instance instead of being called on a new instance
void Klass::instantiate(const std::vector<Value::Ptr>& constructor_arguments) {
	assert(constructor_arguments.size() == m_declaration_stmt.get_constructor_parameters().size());
	m_instance_context = RuntimeContext::make_ptr<RuntimeContext>();
	for(unsigned long i = 0; i < constructor_arguments.size(); ++i) {
		if(!m_instance_context->declare_var(
				   m_declaration_stmt.get_constructor_parameters()[i].name.get_value().as_string(),
				   constructor_arguments[i],
				   m_declaration_stmt.get_constructor_parameters()[i].is_mutable))
			assert(false);
	}
	for(const auto& decl : m_declaration_stmt.get_declarations()) {
		Value::Ptr init = Value::make_ptr<Nothing>();
		if(decl->get_initializer() != nullptr)
			init = Runtime::the().execute_expression(decl->get_initializer(), m_instance_context);
		if(!m_instance_context->declare_var(decl->get_identifier().get_value().as_string(), init, decl->is_mutable()))
			assert(false);
	}
}

Value::Ptr Klass::get_declaration(const std::string& name) const {
	const auto res = m_instance_context->get_var(name);
	assert(res.has_value());
	return res->value;
}
}