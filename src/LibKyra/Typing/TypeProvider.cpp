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
	const Type::Ptr bool_type = Type::make_ptr<ClassType>(Value::NativeTypes::Bool);
	const Type::Ptr number_type = Type::make_ptr<ClassType>(Value::NativeTypes::Number);
	const Type::Ptr string_type = Type::make_ptr<ClassType>(Value::NativeTypes::String);
	const Type::Ptr nothing_type = Type::make_ptr<NothingType>();

	m_native_type_reprs = {encode(bool_type), encode(number_type), encode(string_type), encode(nothing_type)};

	const auto& a_to_b = [&](const Type::Repr& a, const Type::Repr& b) {
		return Variable<Type::Repr>(encode(Type::make_ptr<FunctionType>(b, std::vector<Type::Repr>{a})), true);
	};

	std::unordered_map<std::string, Variable<Type::Repr>> num_decls = {
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
	std::unordered_map<std::string, Variable<Type::Repr>> string_decls = {
			{"operator+", a_to_b(Value::NativeTypes::String, Value::NativeTypes::String)},
			{"operator*", a_to_b(Value::NativeTypes::Number, Value::NativeTypes::String)},
			{"operator<", a_to_b(Value::NativeTypes::String, Value::NativeTypes::Bool)},
			{"operator<=", a_to_b(Value::NativeTypes::String, Value::NativeTypes::Bool)},
			{"operator>", a_to_b(Value::NativeTypes::String, Value::NativeTypes::Bool)},
			{"operator>=", a_to_b(Value::NativeTypes::String, Value::NativeTypes::Bool)},
			{"operator==", a_to_b(Value::NativeTypes::String, Value::NativeTypes::Bool)},
			{"operator!=", a_to_b(Value::NativeTypes::String, Value::NativeTypes::Bool)},
	};
	std::unordered_map<std::string, Variable<Type::Repr>> bool_decls = {
			{"operator<", a_to_b(Value::NativeTypes::Bool, Value::NativeTypes::Bool)},
			{"operator<=", a_to_b(Value::NativeTypes::Bool, Value::NativeTypes::Bool)},
			{"operator>", a_to_b(Value::NativeTypes::Bool, Value::NativeTypes::Bool)},
			{"operator>=", a_to_b(Value::NativeTypes::Bool, Value::NativeTypes::Bool)},
			{"operator==", a_to_b(Value::NativeTypes::Bool, Value::NativeTypes::Bool)},
			{"operator!=", a_to_b(Value::NativeTypes::Bool, Value::NativeTypes::Bool)},
	};
	encode(Type::make_ptr<ClassType>(Value::NativeTypes::Number, num_decls));
	encode(Type::make_ptr<ClassType>(Value::NativeTypes::String, string_decls));
	encode(Type::make_ptr<ClassType>(Value::NativeTypes::Bool, bool_decls));
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
	Type::Repr repr = type->get_name();
	if(auto it = m_types.find(repr); it == m_types.end())
		m_types.try_emplace(repr, type);
	else
		it->second = type;
	return repr;
}

std::vector<Type::Repr> TypeProvider::native_types() { return the().m_native_type_reprs; }
}