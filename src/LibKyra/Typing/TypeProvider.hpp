#pragma once

#include <string>
#include <unordered_map>
#include <vector>

#include "Type.hpp"

namespace Kyra {
class TypeProvider {
public:
	static TypeProvider& the();

	TypeProvider(TypeProvider const&) = delete;
	TypeProvider(TypeProvider&&) noexcept = delete;
	TypeProvider& operator=(TypeProvider const&) = delete;
	TypeProvider& operator=(TypeProvider&&) = delete;

	Type::Ptr decode(const Type::Repr& repr) const;

	Type::Repr encode(Type::Ptr type);

	static std::vector<Type::Repr> native_types();

private:
	TypeProvider();

	std::unordered_map<Type::Repr, Type::Ptr> m_types{};
	std::vector<Type::Repr> m_native_type_reprs;
};
}