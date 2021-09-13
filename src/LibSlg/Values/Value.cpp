#include "Value.hpp"

namespace LibSlg {
const Value::Type Value::NativeTypes::Nothing = "Nothing";
const Value::Type Value::NativeTypes::Number = "Number";
const Value::Type Value::NativeTypes::Bool = "Bool";
const Value::Type Value::NativeTypes::String = "String";
const Value::Type Value::NativeTypes::Function = "Function";  // FIXME a function is not a native type
const std::vector<Value::Type> Value::NativeTypes::All = { Value::NativeTypes::Nothing, Value::NativeTypes::Number,
	Value::NativeTypes::Bool, Value::NativeTypes::String };
}