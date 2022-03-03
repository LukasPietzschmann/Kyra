#include "Number.hpp"

#include <cassert>
#include <string>

namespace Kyra {
Number::Number(int number) : m_number(number) {}

bool Number::is_implicitly_true() const { return true; }

std::string Number::get_type() const { return Value::NativeTypes::Number; }

std::string Number::to_string() const { return std::to_string(m_number); }

int Number::get_number() const { return m_number; }

bool Number::operator==(const HasPtrAlias::Ptr& other) const {
	if(get_type() != other->get_type())
		return false;
	return m_number == Value::as<Number>(other)->m_number;
}

bool Number::operator<(const HasPtrAlias::Ptr& other) const {
	if(get_type() != other->get_type())
		return false;
	return m_number < Value::as<Number>(other)->m_number;
}

bool Number::operator>(const HasPtrAlias::Ptr& other) const {
	if(get_type() != other->get_type())
		return false;
	return m_number > Value::as<Number>(other)->m_number;
}

Value::Ptr Number::operator+(const HasPtrAlias::Ptr& other) const {
	assert(other->get_type() == Value::NativeTypes::Number);
	return Value::make_ptr<Number>(m_number + Value::as<Number>(other)->m_number);
}

Value::Ptr Number::operator-(const HasPtrAlias::Ptr& other) const {
	assert(other->get_type() == Value::NativeTypes::Number);
	return Value::make_ptr<Number>(m_number - Value::as<Number>(other)->m_number);
}

Value::Ptr Number::operator*(const HasPtrAlias::Ptr& other) const {
	assert(other->get_type() == Value::NativeTypes::Number);
	return Value::make_ptr<Number>(m_number * Value::as<Number>(other)->m_number);
}

Value::Ptr Number::operator/(const HasPtrAlias::Ptr& other) const {
	assert(other->get_type() == Value::NativeTypes::Number);
	return Value::make_ptr<Number>(m_number / Value::as<Number>(other)->m_number);
}
}