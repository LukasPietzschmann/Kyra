#pragma once

#include <string>

#include "../Context.hpp"
#include "../Values/Value.hpp"
#include "../Variable.hpp"

namespace Kyra {
class RuntimeContext : public Context<RuntimeContext, Variable<Value::Ptr>, Value::Ptr> {
public:
	explicit RuntimeContext(RuntimeContext::Ptr parent = nullptr) :
		Context<RuntimeContext, Variable<Value::Ptr>, Value::Ptr>(parent) {}
	~RuntimeContext() override = default;

	using Context::mutate_var;
	bool mutate_var(const std::string& name, Value::Ptr value);
	using Context::declare_var;
	bool declare_var(const std::string& name, Value::Ptr value, bool is_mutable = true);
};
}