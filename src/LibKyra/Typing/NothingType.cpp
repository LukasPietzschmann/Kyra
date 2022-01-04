#include "NothingType.hpp"

namespace Kyra {
NothingType::NothingType() : Type("Nothing") {}

bool NothingType::isApplicableForDeclaration() const { return false; }

bool NothingType::canBeAssignedTo(Type::Ptr) const { return true; }
}