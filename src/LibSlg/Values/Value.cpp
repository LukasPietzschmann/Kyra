#include "Value.hpp"

#include "Nothing.hpp"

namespace LibSlg {
const Value::Type Value::NativeTypes::Nothing = "Nothing";
const Value::Type Value::NativeTypes::Number = "Number";
const Value::Type Value::NativeTypes::Bool = "Bool";
const Value::Type Value::NativeTypes::String = "String";
const Value::Type Value::NativeTypes::Function = "Function";
}