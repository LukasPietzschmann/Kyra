#include "Value.hpp"

namespace LibSlg {
const std::string Value::NativeTypes::Nothing = "Nothing";
const std::string Value::NativeTypes::Number = "Number";
const std::string Value::NativeTypes::Bool = "Bool";
const std::string Value::NativeTypes::String = "String";
const std::string Value::NativeTypes::Function = "Function";  // FIXME a function is not a native type
const std::vector<std::string> Value::NativeTypes::All = {Value::NativeTypes::Nothing,
		Value::NativeTypes::Number,
		Value::NativeTypes::Bool,
		Value::NativeTypes::String};
}