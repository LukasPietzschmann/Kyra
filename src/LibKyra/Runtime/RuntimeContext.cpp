#include "RuntimeContext.hpp"

#include <utility>

namespace Kyra {
bool RuntimeContext::mutate_var(const std::string& name, Value::Ptr value) {
	if(m_variables.contains(name)) {
		m_variables.at(name).value = std::move(value);
		return true;
	} else if(m_parent)
		return m_parent->mutate_var(name, value);
	else
		return false;
}

bool RuntimeContext::declare_var(const std::string& name, Value::Ptr value, bool is_mutable) {
	if(m_variables.contains(name))
		return false;
	m_variables.try_emplace(name, value, is_mutable);
	return true;
}

}