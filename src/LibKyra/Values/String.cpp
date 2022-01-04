#include "String.hpp"

namespace Kyra {
String::String(std::string string) : m_string(std::move(string)) {}

bool String::isImplicitlyTrue() const { return true; }

std::string String::getType() const { return Value::NativeTypes::String; }

std::string String::toString() const { return m_string; }

bool String::operator==(const HasPtrAlias::Ptr& other) const {
	if(getType() != other->getType())
		return false;
	return m_string == Value::as<String>(other)->m_string;
}

bool String::operator<(const HasPtrAlias::Ptr& other) const {
	if(getType() != other->getType())
		return false;
	return m_string.compare(Value::as<String>(other)->m_string) < 0;
}

bool String::operator>(const HasPtrAlias::Ptr& other) const {
	if(getType() != other->getType())
		return false;
	return m_string.compare(Value::as<String>(other)->m_string) > 0;
}

Value::Ptr String::operator+(const HasPtrAlias::Ptr& other) const {
	return Value::makePtr<String>(m_string + other->toString());
}

Value::Ptr String::operator*(const HasPtrAlias::Ptr& other) const {
	assert(other->getType() == Value::NativeTypes::Number);
	std::string output;
	for(int i = 0; i < Value::as<Number>(other)->getNumber(); ++i)
		output += m_string;
	return Value::makePtr<String>(output);
}
}