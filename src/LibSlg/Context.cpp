#include "Context.hpp"

namespace LibSlg {
const std::shared_ptr<Context>& Context::getParent() const { return m_parent; }

Context::ContextValue Context::getVar(const std::string& name) const {
	auto pos = m_variables.find(name);
	if(pos != m_variables.end())
		return pos->second;
	if(m_parent != nullptr)
		return m_parent->getVar(name);
	assert(false);
}

const Klass& Context::getCustomType(const Value::Type& type) const {
	auto it = m_customTypes.find(type);
	if(it != m_customTypes.end())
		return it->second;
	if(m_parent != nullptr)
		return m_parent->getCustomType(type);
	assert(false);
}

void Context::declareVar(const std::string& name, const Value::Ptr& value, bool isMutable) {
	m_variables.emplace(std::make_pair(name, Context::ContextValue(value, isMutable)));
}

void Context::mutate(const std::string& name, Value::Ptr value) {
	assert(m_variables.at(name).isMutable);
	if(m_variables.contains(name))
		m_variables.at(name).value = std::move(value);
	else if(m_parent)
		m_parent->mutate(name, value);
	else
		assert(false);
}

void Context::declareType(const Value::Type& type, const Klass& klass) { m_customTypes.emplace(type, klass); }
}