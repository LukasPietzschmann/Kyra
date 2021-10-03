#pragma once

#include <string_view>

#include "../Values/Value.hpp"
#include "ClassType.hpp"
#include "FunctionType.hpp"
#include "NothingType.hpp"
#include "Type.hpp"

namespace LibSlg {
class NativeTypes {
public:
	static Type::Ptr make(const std::string_view& type) {
		static Type::Ptr boolType = Type::makePtr<ClassType>("Bool");
		static Type::Ptr numberType = Type::makePtr<ClassType>("Number");
		static Type::Ptr stringType = Type::makePtr<ClassType>("String");
		static Type::Ptr nothingType = Type::makePtr<NothingType>();

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