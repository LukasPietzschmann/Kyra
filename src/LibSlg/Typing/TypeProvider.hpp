#pragma once

#include <string>

#include "ClassType.hpp"
#include "FunctionType.hpp"
#include "NothingType.hpp"
#include "Type.hpp"

namespace Slanguage {
class TypeProvider {
public:
	using Repr = std::string;

	static TypeProvider& the() {
		static TypeProvider instance;
		return instance;
	}

	TypeProvider(TypeProvider const&) = delete;
	TypeProvider(TypeProvider&&) noexcept = delete;
	TypeProvider& operator=(TypeProvider const&) = delete;
	TypeProvider& operator=(TypeProvider&&) = delete;

	Type::Ptr decode(const Repr& repr) const {
		const auto& it = m_types.find(repr);
		if(it == m_types.end())
			assert(false);
		return it->second;
	}

	Repr encode(Type::Ptr type) {
		Repr repr = type->getName();
		auto it = m_types.find(repr);
		if(it == m_types.end())
			m_types.emplace(repr, type);
		else
			it->second = type;
		return repr;
	}

	static std::vector<Type::Repr> nativeTypes() { return the().m_native_type_reprs; }

private:
	TypeProvider() {
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

	std::unordered_map<Repr, Type::Ptr> m_types{};
	std::vector<Repr> m_native_type_reprs;
};
}