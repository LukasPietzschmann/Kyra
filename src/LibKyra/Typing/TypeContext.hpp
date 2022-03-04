#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "../Context.hpp"
#include "../Variable.hpp"
#include "Type.hpp"
#include "TypeProvider.hpp"

namespace Kyra {
class TypeContext : public Context<TypeContext, Variable<Type::Repr>, Type::Repr> {
public:
	explicit TypeContext(TypeContext::Ptr parent = nullptr) :
		Context<TypeContext, Variable<Type::Repr>, Type::Repr>(std::move(parent)) {
		for(const auto& nt : TypeProvider::native_types())
			m_types.try_emplace(TypeProvider::the().decode(nt)->get_name(), nt);
	}

	using Context::declare_var;
	bool declare_var(const std::string& name, const Type::Repr& var_type, bool is_mutable);
};
}