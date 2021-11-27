#include "RuntimeContext.hpp"

#include <memory>
#include <type_traits>
#include <unordered_map>

#include "../HasPtrAlias.hpp"
#include "../Values/Value.hpp"
#include "../Variable.hpp"

namespace Slanguage {
bool RuntimeContext::mutateVar(const std::string& name, Value::Ptr value) {
	if(m_variables.contains(name)) {
		m_variables.at(name).value = std::move(value);
		return true;
	} else if(m_parent)
		return m_parent->mutateVar(name, value);
	else
		return false;
}

bool RuntimeContext::declareVar(const std::string& name, Value::Ptr value, bool isMutable) {
	if(m_variables.contains(name))
		return false;
	m_variables.try_emplace(name, value, isMutable);
	return true;
}

}