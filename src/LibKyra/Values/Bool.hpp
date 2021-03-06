#pragma once

#include <string>

#include "Value.hpp"

namespace Kyra {
class Bool : public Value {
public:
	explicit Bool(bool boolean);
	~Bool() override = default;
	bool is_implicitly_true() const override;
	std::string get_type() const override;
	std::string to_string() const override;

	bool operator==(const Value::Ptr& other) const override;
	bool operator<(const Value::Ptr& other) const override;
	bool operator>(const Value::Ptr& other) const override;

private:
	bool m_bool;
};
}