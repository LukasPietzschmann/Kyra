#pragma once

#include "Value.hpp"
#include "../Context.hpp"

namespace LibSlg {
class Object : public Value {
public:
	explicit Object(const Context::Ptr& context) : m_context(context) {}
	~Object() override {}
	bool isImplicitlyTrue() const override { return true; }
	Type getType() const override { return Value::NativeTypes::Object; }
	std::string toString() const override { return "[Object]"; }
	const Context::Ptr& getContext() const { return m_context; }

	bool operator==(const Value::Ptr& other) const override {
		if(getType() != other->getType())
			return false;
		return m_context == Value::as<Object>(other)->m_context;
	}

	bool operator<(const Value::Ptr&) const override { return false; }
	bool operator>(const Value::Ptr&) const override { return false; }
private:
	Context::Ptr m_context;
};
}