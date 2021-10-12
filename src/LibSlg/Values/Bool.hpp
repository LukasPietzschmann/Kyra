#pragma once

#include "Value.hpp"

namespace LibSlg {
class Bool : public Value {
public:
	explicit Bool(bool boolean) : m_bool(boolean) {}
	~Bool() override = default;
	bool isImplicitlyTrue() const override { return m_bool; }
	std::string getType() const override { return Value::NativeTypes::Bool; }
	std::string toString() const override { return m_bool ? "true" : "false"; }

	bool operator==(const Value::Ptr& other) const override { return m_bool == other->isImplicitlyTrue(); }
	bool operator<(const Value::Ptr& other) const override { return !m_bool && other->isImplicitlyTrue(); }
	bool operator>(const Value::Ptr& other) const override { return m_bool && !other->isImplicitlyTrue(); }

private:
	bool m_bool;
};
}