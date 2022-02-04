#pragma once

#include <string>

#include "Value.hpp"

namespace Kyra {
class Bool : public Value {
public:
	explicit Bool(bool boolean);
	~Bool() override = default;
	bool isImplicitlyTrue() const override;
	std::string getType() const override;
	std::string toString() const override;

	bool operator==(const Value::Ptr& other) const override;
	bool operator<(const Value::Ptr& other) const override;
	bool operator>(const Value::Ptr& other) const override;

private:
	bool m_bool;
};
}