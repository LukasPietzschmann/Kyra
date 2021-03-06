#pragma once

#include "Value.hpp"

namespace Kyra {
class Number : public Value {
public:
	explicit Number(int number);
	~Number() override = default;

	bool is_implicitly_true() const override;
	std::string get_type() const override;
	std::string to_string() const override;

	int get_number() const;

	bool operator==(const Value::Ptr& other) const override;
	bool operator<(const Value::Ptr& other) const override;
	bool operator>(const Value::Ptr& other) const override;
	Value::Ptr operator+(const Value::Ptr& other) const override;
	Value::Ptr operator-(const Value::Ptr& other) const override;
	Value::Ptr operator*(const Value::Ptr& other) const override;
	Value::Ptr operator/(const Value::Ptr& other) const override;

private:
	int m_number;
};
}