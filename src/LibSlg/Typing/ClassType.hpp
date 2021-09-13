#pragma once

#include <string>
#include <unordered_map>
#include <utility>

#include "Type.hpp"

namespace LibSlg {
class ClassType : public Type {
public:
	ClassType(const std::string& name, bool isMutable, std::unordered_map<std::string, Type::Ptr> declarations = {}) :
		Type(name, isMutable), m_declarations(std::move(declarations)) {}
	~ClassType() override = default;
	Type::Ptr knowsAbout(const std::string& name) const override {
		auto it = m_declarations.find(name);
		if(it == m_declarations.end())
			return nullptr;
		return it->second;
	}

protected:
	std::unordered_map<std::string, Type::Ptr> m_declarations;
};
}