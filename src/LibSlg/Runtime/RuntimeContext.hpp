#pragma once

#include <unordered_map>
#include <utility>
#include <vector>

#include "../Exceptions.hpp"
#include "../Values/Klass.hpp"
#include "../Values/Nothing.hpp"
#include "../Variable.hpp"

namespace Slanguage {
class RuntimeContext {
public:
	explicit RuntimeContext(RuntimeContext* parent = nullptr) : m_parent(parent) {}
	~RuntimeContext() { delete m_parent; }

	Variable<Value::Ptr> getVar(const std::string& name) const;
	Value::Ptr getCustomType(const std::string& type) const;
	void declareVar(const std::string& name, Value::Ptr value, bool isMutable = true);
	void mutate(const std::string& name, Value::Ptr value);
	void declareType(const std::string& type, Value::Ptr klass);

private:
	RuntimeContext* m_parent;
	std::unordered_map<std::string, Variable<Value::Ptr>> m_variables;
	std::unordered_map<std::string, Value::Ptr> m_customTypes;
};
}