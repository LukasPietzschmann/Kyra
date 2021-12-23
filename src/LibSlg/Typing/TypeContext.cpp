#include "TypeContext.hpp"

#include "../HasPtrAlias.hpp"

namespace Slanguage {
bool TypeContext::declareVar(const std::string& name, Type::Repr varType, bool isMutable) {
	return declareVar(name, Variable(varType, isMutable));
}
}