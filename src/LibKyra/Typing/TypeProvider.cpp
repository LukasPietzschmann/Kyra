#include "TypeProvider.hpp"

#include <cassert>
#include <utility>

#include "../Values/Value.hpp"
#include "../Variable.hpp"
#include "ClassType.hpp"
#include "FunctionType.hpp"
#include "NothingType.hpp"

namespace Kyra {
TypeProvider::TypeProvider() {
	const Type::Ptr boolType = Type::makePtr<ClassType>(Value::NativeTypes::Bool);
	const Type::Ptr numberType = Type::makePtr<ClassType>(Value::NativeTypes::Number);
	const Type::Ptr stringType = Type::makePtr<ClassType>(Value::NativeTypes::String);
	const Type::Ptr nothingType = Type::makePtr<NothingType>();

	m_native_type_reprs = {encode(boolType), encode(numberType), encode(stringType), encode(nothingType)};

	const auto& a_to_b = [&](const Type::Repr& a, const Type::Repr& b) {
		return Variable<Type::Repr>(encode(Type::makePtr<FunctionType>(b, std::vector<Type::Repr>{a})), true);
	};

	std::unordered_map<std::string, Variable<Type::Repr>> numDecls = {
			{"operator+", a_to_b(Value::NativeTypes::Number, Value::NativeTypes::Number)},
			{"operator-", a_to_b(Value::NativeTypes::Number, Value::NativeTypes::Number)},
			{"operator*", a_to_b(Value::NativeTypes::Number, Value::NativeTypes::Number)},
			{"operator/", a_to_b(Value::NativeTypes::Number, Value::NativeTypes::Number)},
			{"operator<", a_to_b(Value::NativeTypes::Number, Value::NativeTypes::Bool)},
			{"operator<=", a_to_b(Value::NativeTypes::Number, Value::NativeTypes::Bool)},
			{"operator>", a_to_b(Value::NativeTypes::Number, Value::NativeTypes::Bool)},
			{"operator>=", a_to_b(Value::NativeTypes::Number, Value::NativeTypes::Bool)},
			{"operator==", a_to_b(Value::NativeTypes::Number, Value::NativeTypes::Bool)},
			{"operator!=", a_to_b(Value::NativeTypes::Number, Value::NativeTypes::Bool)},
	};
	std::unordered_map<std::string, Variable<Type::Repr>> stringDecls = {
			{"operator+", a_to_b(Value::NativeTypes::String, Value::NativeTypes::String)},
			{"operator*", a_to_b(Value::NativeTypes::Number, Value::NativeTypes::String)},
			{"operator<", a_to_b(Value::NativeTypes::String, Value::NativeTypes::Bool)},
			{"operator<=", a_to_b(Value::NativeTypes::String, Value::NativeTypes::Bool)},
			{"operator>", a_to_b(Value::NativeTypes::String, Value::NativeTypes::Bool)},
			{"operator>=", a_to_b(Value::NativeTypes::String, Value::NativeTypes::Bool)},
			{"operator==", a_to_b(Value::NativeTypes::String, Value::NativeTypes::Bool)},
			{"operator!=", a_to_b(Value::NativeTypes::String, Value::NativeTypes::Bool)},
	};
	std::unordered_map<std::string, Variable<Type::Repr>> boolDecls = {
			{"operator<", a_to_b(Value::NativeTypes::Bool, Value::NativeTypes::Bool)},
			{"operator<=", a_to_b(Value::NativeTypes::Bool, Value::NativeTypes::Bool)},
			{"operator>", a_to_b(Value::NativeTypes::Bool, Value::NativeTypes::Bool)},
			{"operator>=", a_to_b(Value::NativeTypes::Bool, Value::NativeTypes::Bool)},
			{"operator==", a_to_b(Value::NativeTypes::Bool, Value::NativeTypes::Bool)},
			{"operator!=", a_to_b(Value::NativeTypes::Bool, Value::NativeTypes::Bool)},
	};
	encode(Type::makePtr<ClassType>(Value::NativeTypes::Number, numDecls));
	encode(Type::makePtr<ClassType>(Value::NativeTypes::String, stringDecls));
	encode(Type::makePtr<ClassType>(Value::NativeTypes::Bool, boolDecls));
}

TypeProvider& TypeProvider::the() {
	static TypeProvider instance;
	return instance;
}

Type::Ptr TypeProvider::decode(const Type::Repr& repr) const {
	const auto& it = m_types.find(repr);
	if(it == m_types.end())
		assert(false);
	return it->second;
}

Type::Repr TypeProvider::encode(Type::Ptr type) {
	Type::Repr repr = type->getName();
	if(auto it = m_types.find(repr); it == m_types.end())
		m_types.try_emplace(repr, type);
	else
		it->second = type;
	return repr;
}

std::vector<Type::Repr> TypeProvider::nativeTypes() { return the().m_native_type_reprs; }
}