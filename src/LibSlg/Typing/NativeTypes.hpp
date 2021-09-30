#pragma once

#include "../Values/Value.hpp"
#include "ClassType.hpp"
#include "FunctionType.hpp"
#include "Type.hpp"

namespace LibSlg {
class NativeTypes {
public:
	static Type::Ptr make(const Value::Type& type) {
		if(type == Value::NativeTypes::Bool)
			return Type::makePtr<ClassType>("Bool");
		if(type == Value::NativeTypes::Number)
			return Type::makePtr<ClassType>("Number");
		if(type == Value::NativeTypes::String)
			return Type::makePtr<ClassType>("String");
		if(type == Value::NativeTypes::Nothing)
			return Type::makePtr<ClassType>("Nothing");
		else
			assert(false);
	}
};
}