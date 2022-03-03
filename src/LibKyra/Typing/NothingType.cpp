#include "NothingType.hpp"

namespace Kyra {
NothingType::NothingType() : Type("Nothing") {}

bool NothingType::is_applicable_for_declaration() const { return false; }

bool NothingType::can_be_assigned_to(Type::Ptr) const { return true; }
}