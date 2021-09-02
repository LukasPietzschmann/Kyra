#pragma once

#include <string>

#include "Nothing.hpp"
#include "Value.hpp"

namespace LibSlg {
class String : public Value {
public:
	explicit String(std::string string) : m_string(std::move(string)) {}
	~String() override {}
	bool isImplicitlyTrue() const override { return true; }
	Type getType() const override { return Value::NativeTypes::String; }
	std::string toString() const override { return m_string; }

	bool operator==(const Value::Ptr& other) const override {
		if(getType() != other->getType())
			return false;
		return m_string.compare(Value::as<String>(other)->m_string) == 0;
	}

	bool operator<(const Value::Ptr& other) const override {
		if(getType() != other->getType())
			return false;
		return m_string.compare(Value::as<String>(other)->m_string) < 0;
	}

	bool operator>(const Value::Ptr& other) const override {
		if(getType() != other->getType())
			return false;
		return m_string.compare(Value::as<String>(other)->m_string) > 0;
	}

	Value::Ptr operator+(const Value::Ptr& other) const override {
		return Value::makePtr<String>(m_string + other->toString());
	}

	Value::Ptr operator*(const Value::Ptr& other) const override {
		assert(other->getType() == Value::NativeTypes::Number);
		std::string output;
		for(int i = 0; i < Value::as<Number>(other)->getNumber(); ++i)
			output += m_string;
		return Value::makePtr<String>(output);
	}

private:
	std::string m_string;
};
}