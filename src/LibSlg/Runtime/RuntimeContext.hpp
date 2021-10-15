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

	RuntimeContext(const RuntimeContext& other) :
		m_parent(new RuntimeContext(other.m_parent)), m_variables(other.m_variables),
		m_customTypes(other.m_customTypes) {}

	RuntimeContext(RuntimeContext&& other) noexcept :
		m_parent(other.m_parent), m_variables(std::move(other.m_variables)),
		m_customTypes(std::move(other.m_customTypes)) {
		other.m_parent = nullptr;
	}

	RuntimeContext& operator=(const RuntimeContext& other) {
		if(this == &other)
			return *this;
		m_parent = new RuntimeContext(*other.m_parent);
		m_variables = other.m_variables;
		m_customTypes = other.m_customTypes;
		return *this;
	}

	RuntimeContext& operator=(RuntimeContext&& other) noexcept {
		if(this == &other)
			return *this;
		m_parent = other.m_parent;
		other.m_parent = nullptr;
		m_variables = std::move(other.m_variables);
		m_customTypes = std::move(other.m_customTypes);
		return *this;
	}

	Variable<Value::Ptr> getVar(const std::string& name) const;
	const Klass& getCustomType(const std::string& type) const;
	void declareVar(const std::string& name, const Value::Ptr& value, bool isMutable = true);
	void mutate(const std::string& name, Value::Ptr value);
	void declareType(const std::string& type, const Klass& klass);

private:
	RuntimeContext* m_parent;
	std::unordered_map<std::string, Variable<Value::Ptr>> m_variables;
	std::unordered_map<std::string, Klass> m_customTypes;
};
}