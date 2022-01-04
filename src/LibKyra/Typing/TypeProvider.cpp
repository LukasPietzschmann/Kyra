#include "TypeProvider.hpp"

namespace Kyra {
TypeProvider::TypeProvider() {
	const Type::Ptr boolType = Type::makePtr<ClassType>(Value::NativeTypes::Bool);
	const Type::Ptr numberType = Type::makePtr<ClassType>(Value::NativeTypes::Number);
	const Type::Ptr stringType = Type::makePtr<ClassType>(Value::NativeTypes::String);
	const Type::Ptr nothingType = Type::makePtr<NothingType>();

	m_native_type_reprs = {encode(boolType), encode(numberType), encode(stringType), encode(nothingType)};

	Type::Ptr function = Type::makePtr<FunctionType>(Value::NativeTypes::Number,
			std::vector<Type::Repr>{Value::NativeTypes::Number});
	std::unordered_map<std::string, Variable<Type::Repr>> declarations = {
			{"operator+", Variable<Type::Repr>(encode(function), true)}};
	encode(Type::makePtr<ClassType>(Value::NativeTypes::Number, declarations));
}

TypeProvider& TypeProvider::the() {
	static TypeProvider instance;
	return instance;
}

Type::Ptr TypeProvider::decode(const TypeProvider::Repr& repr) const {
	const auto& it = m_types.find(repr);
	if(it == m_types.end())
		assert(false);
	return it->second;
}

TypeProvider::Repr TypeProvider::encode(Type::Ptr type) {
	Repr repr = type->getName();
	if(auto it = m_types.find(repr); it == m_types.end())
		m_types.try_emplace(repr, type);
	else
		it->second = type;
	return repr;
}

std::vector<Type::Repr> TypeProvider::nativeTypes() { return the().m_native_type_reprs; }
}