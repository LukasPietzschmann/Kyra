#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "../Context.hpp"
#include "../Variable.hpp"
#include "ClassType.hpp"
#include "FunctionType.hpp"
#include "NothingType.hpp"
#include "Type.hpp"

namespace Kyra {
class TypeContext : public Context<TypeContext, Variable<Type::Ptr>, Type::Ptr> {
public:
	explicit TypeContext(TypeContext::Ptr parent = nullptr);

	using Context::declare_var;
	bool declare_var(const std::string& name, Type::Ptr var_type, bool is_mutable);
};
}