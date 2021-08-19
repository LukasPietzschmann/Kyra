#pragma once

#include <unordered_map>
#include <utility>
#include "Exceptions.hpp"
#include "Value.hpp"

namespace LibSlg {
class Context {
public:
	explicit Context(std::shared_ptr<Context> parent=nullptr) : m_parent(std::move(parent)) {}

	Value::Ptr get(const std::string& name);
	void declare(const std::string& name, Value::Ptr value = Value::makePtr());
	void mutate(const std::string& name, Value::Ptr value);
private:
	const std::shared_ptr<Context> m_parent;
	std::unordered_map<std::string, Value::Ptr> m_variables;
};
};