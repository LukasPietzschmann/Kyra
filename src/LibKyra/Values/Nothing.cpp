#include "Nothing.hpp"

namespace Kyra {
bool Nothing::isImplicitlyTrue() const { return false; }

std::string Nothing::getType() const { return Value::NativeTypes::Nothing; }

bool Nothing::hasCorrectTypeForAssignment(const std::string&) const { return true; }

std::string Nothing::toString() const { return "nothing"; }

bool Nothing::operator==(const HasPtrAlias::Ptr& other) const {
	if(getType() != other->getType())
		return false;
	return true;
}

bool Nothing::operator<(const HasPtrAlias::Ptr&) const { return false; }

bool Nothing::operator>(const HasPtrAlias::Ptr&) const { return false; }
}