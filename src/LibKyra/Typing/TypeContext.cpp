#include "TypeContext.hpp"

namespace Kyra {
bool TypeContext::declare_var(const std::string& name, const Type::Repr& var_type, bool is_mutable) {
	return declare_var(name, Variable(var_type, is_mutable));
}
}