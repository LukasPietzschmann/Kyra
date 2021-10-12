#pragma once

#include <ostream>

#include "Value.hpp"

namespace LibSlg {
class Nothing : public Value {
public:
	Nothing() = default;
	~Nothing() override = default;
	bool isImplicitlyTrue() const override { return false; }
	std::string getType() const override { return Value::NativeTypes::Nothing; }
	bool hasCorrectTypeForAssignment(const std::string&) const override { return true; }
	std::string toString() const override { return "nothing"; }

	bool operator==(const Value::Ptr& other) const override {
		if(getType() != other->getType())
			return false;
		return true;
	}

	bool operator<(const Value::Ptr&) const override { return false; }
	bool operator>(const Value::Ptr&) const override { return false; }
};
}