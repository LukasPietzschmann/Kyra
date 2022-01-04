#pragma once

#include <string>

#include "ClassType.hpp"
#include "FunctionType.hpp"
#include "NothingType.hpp"
#include "Type.hpp"

namespace Kyra {
class TypeProvider {
public:
	using Repr = std::string;

	static TypeProvider& the();

	TypeProvider(TypeProvider const&) = delete;
	TypeProvider(TypeProvider&&) noexcept = delete;
	TypeProvider& operator=(TypeProvider const&) = delete;
	TypeProvider& operator=(TypeProvider&&) = delete;

	Type::Ptr decode(const Repr& repr) const;

	Repr encode(Type::Ptr type);

	static std::vector<Type::Repr> nativeTypes();

private:
	TypeProvider();

	std::unordered_map<Repr, Type::Ptr> m_types{};
	std::vector<Repr> m_native_type_reprs;
};
}