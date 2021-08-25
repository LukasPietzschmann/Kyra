#pragma once

#include <ostream>
#include "Value.hpp"

namespace LibSlg {
class Number : public Value {
public:
	explicit Number(int number) : m_number(number) {}
	~Number() override {}
	bool isImplicitlyTrue() const override { return true; }
	Type getType() const override { return Value::NativeTypes::Number; }
	std::string toString() const override { return std::to_string(m_number); }

	int getNumber() const { return m_number; }

	bool operator==(const Value::Ptr& other) const override {
		if(getType() != other->getType())
			return false;
		return m_number == Value::as<Number>(other)->m_number;
	}

	bool operator<(const Value::Ptr& other) const override {
		if(getType() != other->getType())
			return false;
		return m_number < Value::as<Number>(other)->m_number;
	}

	bool operator>(const Value::Ptr& other) const override {
		if(getType() != other->getType())
			return false;
		return m_number > Value::as<Number>(other)->m_number;
	}

	Value::Ptr operator+(const Value::Ptr& other) const override {
		assert(other->getType() == Value::NativeTypes::Number);
		return Value::makePtr<Number>(m_number + Value::as<Number>(other)->m_number);
	}

	Value::Ptr operator-(const Value::Ptr& other) const override {
		assert(other->getType() == Value::NativeTypes::Number);
		return Value::makePtr<Number>(m_number - Value::as<Number>(other)->m_number);
	}

	Value::Ptr operator*(const Value::Ptr& other) const override {
		assert(other->getType() == Value::NativeTypes::Number);
		return Value::makePtr<Number>(m_number * Value::as<Number>(other)->m_number);
	}

	Value::Ptr operator/(const Value::Ptr& other) const override {
		assert(other->getType() == Value::NativeTypes::Number);
		return Value::makePtr<Number>(m_number / Value::as<Number>(other)->m_number);
	}
private:
	int m_number;
};
}