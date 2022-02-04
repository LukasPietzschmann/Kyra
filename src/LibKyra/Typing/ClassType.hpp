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
			std::vector<Type::Repr> constructorParams = {}) :
		Type(name),
		m_declarations(std::move(declarations)), m_constructorParameter(std::move(constructorParams)) {}
	~ClassType() override = default;

	std::optional<Variable<Type::Repr>> knowsAbout(const std::string& name) const override;

	void addConstructorParam(const Type::Repr& param);
	void addDeclaration(const std::string& name, const Variable<Type::Repr>& value);
	const std::vector<Type::Repr>& getConstructorParameter() const;
	unsigned long getArity() const;

private:
	std::unordered_map<std::string, Variable<Type::Repr>> m_declarations;
	std::vector<Type::Repr> m_constructorParameter;
};
}