#pragma once

#include <iosfwd>
#include <string>
#include <unordered_map>

#include "../HasPtrAlias.hpp"
#include "../Values/Value.hpp"
#include "../Variable.hpp"

namespace Slanguage {
class RuntimeContext : public HasPtrAlias<RuntimeContext> {
public:
	explicit RuntimeContext(RuntimeContext::Ptr parent = nullptr) : m_parent(parent) {}

	Variable<Value::Ptr> getVar(const std::string& name) const;
	Value::Ptr getCustomType(const std::string& type) const;
	void declareVar(const std::string& name, Value::Ptr value, bool isMutable = true);
	void mutate(const std::string& name, Value::Ptr value);
	void declareType(const std::string& type, Value::Ptr klass);

private:
	RuntimeContext::Ptr m_parent;
	std::unordered_map<std::string, Variable<Value::Ptr>> m_variables;
	std::unordered_map<std::string, Value::Ptr> m_customTypes;
};
}