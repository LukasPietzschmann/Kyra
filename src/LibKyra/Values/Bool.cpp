#include "Bool.hpp"

namespace Kyra {
Bool::Bool(bool boolean) : m_bool(boolean) {}

bool Bool::is_implicitly_true() const { return m_bool; }

std::string Bool::get_type() const { return Value::NativeTypes::Bool; }

std::string Bool::to_string() const { return m_bool ? "true" : "false"; }

bool Bool::operator==(const HasPtrAlias::Ptr& other) const { return m_bool == other->is_implicitly_true(); }

bool Bool::operator<(const HasPtrAlias::Ptr& other) const { return !m_bool && other->is_implicitly_true(); }

bool Bool::operator>(const HasPtrAlias::Ptr& other) const { return m_bool && !other->is_implicitly_true(); }
}