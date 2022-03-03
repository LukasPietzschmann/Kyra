#pragma once

#include "Value.hpp"

namespace Kyra {
class Nothing : public Value {
public:
	Nothing() = default;
	~Nothing() override = default;
	bool is_implicitly_true() const override;
	std::string get_type() const override;
	std::string to_string() const override;

	bool operator==(const Value::Ptr& other) const override;

	bool operator<(const Value::Ptr&) const override;
	bool operator>(const Value::Ptr&) const override;
};
}