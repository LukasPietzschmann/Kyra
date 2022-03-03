#pragma once

#include <string>

#include "Value.hpp"

namespace Kyra {
class String : public Value {
public:
	explicit String(std::string string);
	~String() override = default;

	bool is_implicitly_true() const override;
	std::string get_type() const override;
	std::string to_string() const override;

	bool operator==(const Value::Ptr& other) const override;
	bool operator<(const Value::Ptr& other) const override;
	bool operator>(const Value::Ptr& other) const override;
	Value::Ptr operator+(const Value::Ptr& other) const override;
	Value::Ptr operator*(const Value::Ptr& other) const override;

private:
	std::string m_string;
};
}