#include "ClassType.hpp"

namespace Kyra {
std::optional<Variable<Type::Ptr>> ClassType::knows_about(const std::string& name) const {
	auto it = m_declarations.find(name);
	if(it == m_declarations.end())
		return {};
	return it->second;
}

void ClassType::add_constructor_param(Type::Ptr param) { m_constructor_parameter.push_back(std::move(param)); }

void ClassType::add_declaration(const std::string& name, const Variable<Type::Ptr>& value) {
	m_declarations.try_emplace(name, value);
}

const std::vector<Type::Ptr>& ClassType::get_constructor_parameter() const { return m_constructor_parameter; }

unsigned long ClassType::get_arity() const { return m_constructor_parameter.size(); }
}