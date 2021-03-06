#include "RuntimeContext.hpp"

#include <utility>

namespace Kyra {
bool RuntimeContext::mutate_var(const std::string& name, Value::Ptr value) {
	if(const auto& it = m_variables.find(name); it != m_variables.end()) {
		it->second.value = std::move(value);
		return true;
	} else if(m_parent)
		return m_parent->mutate_var(name, std::move(value));
	else
		return false;
}

bool RuntimeContext::declare_var(const std::string& name, Value::Ptr value, bool is_mutable) {
	return declare_var(name, Variable(std::move(value), is_mutable));
}

}