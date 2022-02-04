#include "Number.hpp"

#include <cassert>
#include <string>

namespace Kyra {
Number::Number(int number) : m_number(number) {}

bool Number::isImplicitlyTrue() const { return true; }

std::string Number::getType() const { return Value::NativeTypes::Number; }

std::string Number::toString() const { return std::to_string(m_number); }

int Number::getNumber() const { return m_number; }

bool Number::operator==(const HasPtrAlias::Ptr& other) const {
	if(getType() != other->getType())
		return false;
	return m_number == Value::as<Number>(other)->m_number;
}

bool Number::operator<(const HasPtrAlias::Ptr& other) const {
	if(getType() != other->getType())
		return false;
	return m_number < Value::as<Number>(other)->m_number;
}

bool Number::operator>(const HasPtrAlias::Ptr& other) const {
	if(getType() != other->getType())
		return false;
	return m_number > Value::as<Number>(other)->m_number;
}

Value::Ptr Number::operator+(const HasPtrAlias::Ptr& other) const {
	assert(other->getType() == Value::NativeTypes::Number);
	return Value::makePtr<Number>(m_number + Value::as<Number>(other)->m_number);
}

Value::Ptr Number::operator-(const HasPtrAlias::Ptr& other) const {
	assert(other->getType() == Value::NativeTypes::Number);
	return Value::makePtr<Number>(m_number - Value::as<Number>(other)->m_number);
}

Value::Ptr Number::operator*(const HasPtrAlias::Ptr& other) const {
	assert(other->getType() == Value::NativeTypes::Number);
	return Value::makePtr<Number>(m_number * Value::as<Number>(other)->m_number);
}

Value::Ptr Number::operator/(const HasPtrAlias::Ptr& other) const {
	assert(other->getType() == Value::NativeTypes::Number);
	return Value::makePtr<Number>(m_number / Value::as<Number>(other)->m_number);
}
}