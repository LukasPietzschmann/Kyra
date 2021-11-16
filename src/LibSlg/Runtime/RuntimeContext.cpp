#include "RuntimeContext.hpp"

#include <cassert>
#include <memory>
#include <type_traits>
#include <utility>

#include "../HasPtrAlias.hpp"
#include "../Values/Value.hpp"
#include "../Variable.hpp"

namespace Slanguage {
Variable<Value::Ptr> RuntimeContext::getVar(const std::string& name) const {
	if(const auto& pos = m_variables.find(name); pos != m_variables.end())
		return pos->second;
	if(m_parent != nullptr)
		return m_parent->getVar(name);
	assert(false);
}

Value::Ptr RuntimeContext::getCustomType(const std::string& type) const {
	if(const auto& it = m_customTypes.find(type); it != m_customTypes.end())
		return it->second;
	if(m_parent != nullptr)
		return m_parent->getCustomType(type);
	assert(false);
}

bool RuntimeContext::declareVar(const std::string& name, Value::Ptr value, bool isMutable) {
	if(m_variables.contains(name))
		return false;
	m_variables.try_emplace(name, value, isMutable);
	return true;
}

bool RuntimeContext::mutate(const std::string& name, Value::Ptr value) {
	if(m_variables.contains(name)) {
		m_variables.at(name).value = std::move(value);
		return true;
	} else if(m_parent)
		m_parent->mutate(name, value);
	else
		return false;
}

void RuntimeContext::declareType(const std::string& type, Value::Ptr klass) { m_customTypes.try_emplace(type, klass); }
}