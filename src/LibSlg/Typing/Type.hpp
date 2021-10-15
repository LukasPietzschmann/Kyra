#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <utility>

#include "../HasPtrAlias.hpp"
#include "../Values/Function.hpp"
#include "../Values/Value.hpp"
#include "../Variable.hpp"

namespace Slanguage {
class Type : public HasPtrAlias<Type> {
public:
	explicit Type(std::string name) : m_name(std::move(name)) {}
	virtual ~Type() = default;

	virtual std::optional<Variable<Type::Ptr>> knowsAbout(const std::string&) const { return {}; }
	virtual bool canBeCalledWith(const std::vector<Ptr>&) const { return false; };
	virtual bool isApplicableForDeclaration() const { return true; }
	virtual bool canBeAssignedTo(const Type::Ptr& assignee) const { return *this == assignee; };

	const std::string& getName() const { return m_name; }

	virtual bool operator==(const Type::Ptr& other) const { return m_name == other->m_name; }
	virtual bool operator!=(const Type::Ptr& other) const { return !(*this == other); }

protected:
	std::string m_name;
};
}