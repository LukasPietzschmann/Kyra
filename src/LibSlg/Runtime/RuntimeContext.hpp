#pragma once

#include <unordered_map>
#include <utility>
#include <vector>

#include "../Exceptions.hpp"
#include "../Values/Klass.hpp"
#include "../Values/Nothing.hpp"

namespace LibSlg {
class RuntimeContext {
public:
	struct ContextValue {
		ContextValue(Value::Ptr value, bool isMutable) : value(std::move(value)), isMutable(isMutable) {}
		Value::Ptr value;
		bool isMutable;
	};
	explicit RuntimeContext(RuntimeContext* parent = nullptr) : m_parent(parent) {}

	RuntimeContext* getParent() const;
	ContextValue getVar(const std::string& name) const;
	const Klass& getCustomType(const Value::Type& type) const;
	void declareVar(const std::string& name, const Value::Ptr& value, bool isMutable = true);
	void mutate(const std::string& name, Value::Ptr value);
	void declareType(const Value::Type& type, const Klass& klass);

private:
	RuntimeContext* m_parent;
	std::unordered_map<std::string, ContextValue> m_variables;
	std::unordered_map<Value::Type, Klass> m_customTypes;
};
}