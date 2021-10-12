#pragma once

#include <unordered_map>
#include <utility>
#include <vector>

#include "../Exceptions.hpp"
#include "../Values/Klass.hpp"
#include "../Values/Nothing.hpp"
#include "../Variable.hpp"

namespace LibSlg {
class RuntimeContext {
public:
	explicit RuntimeContext(RuntimeContext* parent = nullptr) : m_parent(parent) {}

	RuntimeContext* getParent() const;
	Variable<Value::Ptr> getVar(const std::string& name) const;
	const Klass& getCustomType(const Value::Type& type) const;
	void declareVar(const std::string& name, const Value::Ptr& value, bool isMutable = true);
	void mutate(const std::string& name, Value::Ptr value);
	void declareType(const Value::Type& type, const Klass& klass);

private:
	RuntimeContext* m_parent;
	std::unordered_map<std::string, Variable<Value::Ptr>> m_variables;
	std::unordered_map<Value::Type, Klass> m_customTypes;
};
}