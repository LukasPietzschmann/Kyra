#include "String.hpp"

#include <cassert>
#include <utility>

#include "Number.hpp"

namespace Kyra {
String::String(std::string string) : m_string(std::move(string)) {}

bool String::is_implicitly_true() const { return true; }

std::string String::get_type() const { return Value::NativeTypes::String; }

std::string String::to_string() const { return m_string; }

bool String::operator==(const HasPtrAlias::Ptr& other) const {
	if(get_type() != other->get_type())
		return false;
	return m_string == Value::as<String>(other)->m_string;
}

bool String::operator<(const HasPtrAlias::Ptr& other) const {
	if(get_type() != other->get_type())
		return false;
	return m_string.compare(Value::as<String>(other)->m_string) < 0;
}

bool String::operator>(const HasPtrAlias::Ptr& other) const {
	if(get_type() != other->get_type())
		return false;
	return m_string.compare(Value::as<String>(other)->m_string) > 0;
}

Value::Ptr String::operator+(const HasPtrAlias::Ptr& other) const {
	return Value::make_ptr<String>(m_string + other->to_string());
}

Value::Ptr String::operator*(const HasPtrAlias::Ptr& other) const {
	assert(other->get_type() == Value::NativeTypes::Number);
	std::string output;
	for(int i = 0; i < Value::as<Number>(other)->get_number(); ++i)
		output += m_string;
	return Value::make_ptr<String>(output);
}
}