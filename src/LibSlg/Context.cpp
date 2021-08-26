#include "Context.hpp"

#include <utility>

namespace LibSlg {
const std::shared_ptr<Context>& Context::getParent() const { return m_parent; }

Context::ContextValue Context::get(const std::string& name) {
	auto pos = m_variables.find(name);
	if(pos != m_variables.end())
		return pos->second;
	if(m_parent != nullptr)
		return m_parent->get(name);
	throw RuntimeException("Undefined variable " + name);
}

void Context::declare(const std::string& name, Value::Ptr value, Value::Type type, bool isMutable) {
	if(m_variables.contains(name))
		throw RuntimeException("Variable " + name + " already declared");
	m_variables.emplace(std::make_pair(name, Context::ContextValue(std::move(value), std::move(type), isMutable)));
}

void Context::mutate(const std::string& name, Value::Ptr value) {
	assert(m_variables.at(name).isMutable);
	if(m_variables.contains(name))
		m_variables.at(name).value = std::move(value);
	else if(m_parent)
		m_parent->mutate(name, value);
	else
		throw RuntimeException("Undefined variable " + name);
}

bool Context::isTypeKnown(const Value::Type& type) {
	if(std::find(m_types.begin(), m_types.end(), type) != m_types.end())
		return true;
	if(m_parent != nullptr)
		return m_parent->isTypeKnown(type);
	return false;
}

bool Context::declareType(const Value::Type& type) {
	if(isTypeKnown(type))
		return false;
	m_types.emplace_back(type);
}
}