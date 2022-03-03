#include "Nothing.hpp"

#include <string>

namespace Kyra {
bool Nothing::is_implicitly_true() const { return false; }

std::string Nothing::get_type() const { return Value::NativeTypes::Nothing; }

std::string Nothing::to_string() const { return "nothing"; }

bool Nothing::operator==(const HasPtrAlias::Ptr& other) const {
	if(get_type() != other->get_type())
		return false;
	return true;
}

bool Nothing::operator<(const HasPtrAlias::Ptr&) const { return false; }

bool Nothing::operator>(const HasPtrAlias::Ptr&) const { return false; }
}