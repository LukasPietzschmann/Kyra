#include "Bool.hpp"

namespace Kyra {
Bool::Bool(bool boolean) : m_bool(boolean) {}

bool Bool::isImplicitlyTrue() const { return m_bool; }

std::string Bool::getType() const { return Value::NativeTypes::Bool; }

std::string Bool::toString() const { return m_bool ? "true" : "false"; }

bool Bool::operator==(const HasPtrAlias::Ptr& other) const { return m_bool == other->isImplicitlyTrue(); }

bool Bool::operator<(const HasPtrAlias::Ptr& other) const { return !m_bool && other->isImplicitlyTrue(); }

bool Bool::operator>(const HasPtrAlias::Ptr& other) const { return m_bool && !other->isImplicitlyTrue(); }
}