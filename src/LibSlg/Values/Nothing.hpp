#pragma once

#include <ostream>
#include "Value.hpp"

namespace LibSlg {
class Nothing : public Value {
public:
	Nothing() = default;
	~Nothing() override {}
	bool isImplicitlyTrue() const override { return false; }
	Type getType() const override { return Value::NativeTypes::Nothing; }
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