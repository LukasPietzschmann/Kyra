#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "../Context.hpp"
#include "../Variable.hpp"
#include "Type.hpp"
#include "TypeProvider.hpp"

namespace Kyra {
class TypeContext : public Context<TypeContext, Variable<Type::Repr>, Type::Repr> {
public:
	explicit TypeContext(TypeContext::Ptr parent = nullptr) :
		Context<TypeContext, Variable<Type::Repr>, Type::Repr>(parent) {
		for(const auto& nt : TypeProvider::nativeTypes())
			m_types.try_emplace(TypeProvider::the().decode(nt)->getName(), nt);
	}

	using Context::declareVar;
	bool declareVar(const std::string& name, const Type::Repr& varType, bool isMutable);
};
}