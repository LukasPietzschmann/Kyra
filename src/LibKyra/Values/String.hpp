#pragma once

#include <string>

#include "Nothing.hpp"
#include "Number.hpp"
#include "Value.hpp"

namespace Kyra {
class String : public Value {
public:
	explicit String(std::string string);
	~String() override = default;

	bool isImplicitlyTrue() const override;
	std::string getType() const override;
	std::string toString() const override;

	bool operator==(const Value::Ptr& other) const override;
	bool operator<(const Value::Ptr& other) const override;
	bool operator>(const Value::Ptr& other) const override;
	Value::Ptr operator+(const Value::Ptr& other) const override;
	Value::Ptr operator*(const Value::Ptr& other) const override;

private:
	std::string m_string;
};
}