#pragma once

#include <iosfwd>
#include <string>

#include "../Context.hpp"
#include "../HasPtrAlias.hpp"
#include "../Values/Value.hpp"
#include "../Variable.hpp"

namespace Kyra {
class RuntimeContext : public Context<RuntimeContext, Variable<Value::Ptr>, Value::Ptr> {
public:
	explicit RuntimeContext(RuntimeContext::Ptr parent = nullptr) :
		Context<RuntimeContext, Variable<Value::Ptr>, Value::Ptr>(parent) {}
	~RuntimeContext() override = default;

	using Context::mutateVar;
	bool mutateVar(const std::string& name, Value::Ptr value);
	using Context::declareVar;
	bool declareVar(const std::string& name, Value::Ptr value, bool isMutable = true);
};
}