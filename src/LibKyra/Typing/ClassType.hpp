#pragma once

#include <optional>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "../Variable.hpp"
#include "Type.hpp"

namespace Kyra {
class ClassType : public Type {
public:
	explicit ClassType(const std::string& name,
			std::unordered_map<std::string, Variable<Type::Repr>> declarations = {},
			std::vector<Type::Repr> constructor_params = {}) :
		Type(name),
		m_declarations(std::move(declarations)), m_constructor_parameter(std::move(constructor_params)) {}
	~ClassType() override = default;

	std::optional<Variable<Type::Repr>> knows_about(const std::string& name) const override;

	void add_constructor_param(const Type::Repr& param);
	void add_declaration(const std::string& name, const Variable<Type::Repr>& value);
	const std::vector<Type::Repr>& get_constructor_parameter() const;
	unsigned long get_arity() const;

private:
	std::unordered_map<std::string, Variable<Type::Repr>> m_declarations;
	std::vector<Type::Repr> m_constructor_parameter;
};
}