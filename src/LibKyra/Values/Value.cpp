#include "Value.hpp"

namespace Kyra {
const std::string Value::NativeTypes::Nothing = "Nothing";
const std::string Value::NativeTypes::Number = "Number";
const std::string Value::NativeTypes::Bool = "Bool";
const std::string Value::NativeTypes::String = "String";
const std::string Value::NativeTypes::Function = "Function";  // FIXME a function is not a native type
}