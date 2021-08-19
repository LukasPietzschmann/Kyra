#include "Context.hpp"

#include <utility>

namespace LibSlg {
Value::Ptr Context::get(const std::string& name) {
	auto pos = m_variables.find(name);
	if(pos != m_variables.end())
		return pos->second;
	if(m_parent != nullptr)
		return m_parent->get(name);
	throw RuntimeException("Undefined variable " + name);
}

void Context::declare(const std::string& name, Value::Ptr value) { //FIXME parent context beachten
	if(m_variables.contains(name))
		throw RuntimeException("Variable " + name + " already declared");
	m_variables[name] = std::move(value);
}

void Context::mutate(const std::string& name, Value::Ptr value) { //FIXME parent context beachten
	if(!m_variables.contains(name))
		throw RuntimeException("Undefined variable " + name);
	m_variables[name] = std::move(value);
}
}