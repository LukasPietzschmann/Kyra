#pragma once

#include <ostream>

#include "Value.hpp"

namespace Kyra {
class Nothing : public Value {
public:
	Nothing() = default;
	~Nothing() override = default;
	bool isImplicitlyTrue() const override;
	std::string getType() const override;
	std::string toString() const override;

	bool operator==(const Value::Ptr& other) const override;

	bool operator<(const Value::Ptr&) const override;
	bool operator>(const Value::Ptr&) const override;
};
}