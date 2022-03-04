#include "Type.hpp"

#include <utility>

#include "../Variable.hpp"

namespace Kyra {
Type::Type(std::string name) : m_name(std::move(name)) {}

std::optional<Variable<Type::Repr>> Type::knows_about(const std::string&) const { return {}; }

bool Type::can_be_called_with(const std::vector<Type::Repr>&) const { return false; }

bool Type::is_applicable_for_declaration() const { return true; }

bool Type::can_be_assigned_to(const Type::Ptr& assignee) const { return *this == assignee; }

bool Type::is_function() const { return false; }

const std::string& Type::get_name() const { return m_name; }

bool Type::operator==(const HasPtrAlias::Ptr& other) const { return m_name == other->m_name; }

bool Type::operator!=(const HasPtrAlias::Ptr& other) const { return !(*this == other); }
}