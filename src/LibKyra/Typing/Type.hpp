#pragma once

#include <string>
#include <unordered_map>
#include <utility>

#include "../HasPtrAlias.hpp"
#include "../Values/Function.hpp"
#include "../Values/Value.hpp"
#include "../Variable.hpp"

namespace Kyra {
class Type : public HasPtrAlias<Type> {
public:
	using Repr = std::string;
	explicit Type(std::string name);
	virtual ~Type() = default;

	virtual std::optional<Variable<Type::Repr>> knowsAbout(const std::string&) const;
	virtual bool canBeCalledWith(const std::vector<Type::Repr>&) const;
	virtual bool hasReturnType(const Type::Repr&) const;
	virtual bool isApplicableForDeclaration() const;
	virtual bool canBeAssignedTo(Type::Ptr assignee) const;
	virtual bool isFunction() const;

	const std::string& getName() const;

	virtual bool operator==(const Type::Ptr& other) const;
	virtual bool operator!=(const Type::Ptr& other) const;

protected:
	std::string m_name;
};
}