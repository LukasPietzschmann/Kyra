#include "Type.hpp"

#include <utility>

#include "../Variable.hpp"

namespace Kyra {
Type::Type(std::string name) : m_name(std::move(name)) {}

std::optional<Variable<Type::Repr>> Type::knowsAbout(const std::string&) const { return {}; }

bool Type::canBeCalledWith(const std::vector<Type::Repr>&) const { return false; }

bool Type::hasReturnType(const Type::Repr&) const { return false; }

bool Type::isApplicableForDeclaration() const { return true; }

bool Type::canBeAssignedTo(Type::Ptr assignee) const { return *this == assignee; }

bool Type::isFunction() const { return false; }

const std::string& Type::getName() const { return m_name; }

bool Type::operator==(const HasPtrAlias::Ptr& other) const { return m_name == other->m_name; }

bool Type::operator!=(const HasPtrAlias::Ptr& other) const { return !(*this == other); }
}