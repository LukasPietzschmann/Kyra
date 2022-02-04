#include "TypeContext.hpp"

namespace Kyra {
bool TypeContext::declareVar(const std::string& name, const Type::Repr& varType, bool isMutable) {
	return declareVar(name, Variable(varType, isMutable));
}
}