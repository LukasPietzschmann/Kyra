#pragma once

#include "../Values/Value.hpp"
#include "ClassType.hpp"
#include "FunctionType.hpp"
#include "Type.hpp"

namespace LibSlg {
class NativeTypes {
public:
	static Type::Ptr make(const Value::Type& type) {
		static Type::Ptr boolType = Type::makePtr<ClassType>("Bool");
		static Type::Ptr numberType = Type::makePtr<ClassType>("Number");
		static Type::Ptr stringType = Type::makePtr<ClassType>("String");
		static Type::Ptr nothingType = Type::makePtr<ClassType>("Nothing");

		if(type == Value::NativeTypes::Bool)
			return boolType;
		if(type == Value::NativeTypes::Number)
			return numberType;
		if(type == Value::NativeTypes::String)
			return stringType;
		if(type == Value::NativeTypes::Nothing)
			return nothingType;
		else
			assert(false);
	}
};
}