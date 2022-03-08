#include "Nothing.hpp"

#include <string>

namespace Kyra {
bool Nothing::is_implicitly_true() const { return false; }

std::string Nothing::get_type() const { return Value::NativeTypes::Nothing; }

std::string Nothing::to_string() const { return "nothing"; }
}