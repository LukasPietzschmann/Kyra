#include "RuntimeContext.hpp"

namespace LibSlg {
RuntimeContext* RuntimeContext::getParent() const { return m_parent; }

Variable<Value::Ptr> RuntimeContext::getVar(const std::string& name) const {
	if(const auto& pos = m_variables.find(name); pos != m_variables.end())
		return pos->second;
	if(m_parent != nullptr)
		return m_parent->getVar(name);
	assert(false);
}

const Klass& RuntimeContext::getCustomType(const Value::Type& type) const {
	if(const auto& it = m_customTypes.find(type); it != m_customTypes.end())
		return it->second;
	if(m_parent != nullptr)
		return m_parent->getCustomType(type);
	assert(false);
}

void RuntimeContext::declareVar(const std::string& name, const Value::Ptr& value, bool isMutable) {
	m_variables.try_emplace(name, value, isMutable);
}

void RuntimeContext::mutate(const std::string& name, Value::Ptr value) {
	assert(m_variables.at(name).isMutable);
	if(m_variables.contains(name))
		m_variables.at(name).value = std::move(value);
	else if(m_parent)
		m_parent->mutate(name, value);
	else
		assert(false);
}

void RuntimeContext::declareType(const Value::Type& type, const Klass& klass) {
	m_customTypes.try_emplace(type, klass);
}
}