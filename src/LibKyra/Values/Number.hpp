#pragma once

#include <ostream>

#include "Value.hpp"

namespace Kyra {
class Number : public Value {
public:
	explicit Number(int number);
	~Number() override = default;

	bool isImplicitlyTrue() const override;
	std::string getType() const override;
	std::string toString() const override;

	int getNumber() const;

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