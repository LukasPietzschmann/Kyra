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
			std::unordered_map<std::string, Variable<Type::Ptr>>&& declarations = {},
			std::vector<Type::Ptr>&& constructor_params = {}) :
		Type(name),
		m_declarations(std::move(declarations)), m_constructor_parameter(std::move(constructor_params)) {}
	~ClassType() override = default;

	std::optional<Variable<Type::Ptr>> knows_about(const std::string& name) const override;

	void add_constructor_param(Type::Ptr param);
	void add_declaration(const std::string& name, const Variable<Type::Ptr>& value);
	const std::vector<Type::Ptr>& get_constructor_parameter() const;
	unsigned long get_arity() const;

private:
	std::unordered_map<std::string, Variable<Type::Ptr>> m_declarations;
	std::vector<Type::Ptr> m_constructor_parameter;
};
}