#pragma once
#include <iosfwd>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "../Context.hpp"
#include "../HasPtrAlias.hpp"
#include "../Values/Value.hpp"
#include "../Variable.hpp"
#include "NativeTypes.hpp"
#include "Type.hpp"

namespace Slanguage {
class TypeContext : public Context<TypeContext, Variable<Type::Ptr>, Type::Ptr> {
public:
	explicit TypeContext(TypeContext::Ptr parent = nullptr) :
		Context<TypeContext, Variable<Type::Ptr>, Type::Ptr>(parent) {
		for(const auto& nativeType : Value::NativeTypes::All)
			m_types.try_emplace(nativeType, NativeTypes::make(nativeType));
	}

	using Context::declareVar;
	bool declareVar(const std::string& name, Type::Ptr varType, bool isMutable);
};
}