#include "TypeContext.hpp"

#include <utility>

namespace Kyra {
bool TypeContext::declare_var(const std::string& name, Type::Ptr var_type, bool is_mutable) {
	return declare_var(name, Variable(std::move(var_type), is_mutable));
}
}