#pragma once

#include <optional>
#include <string>
#include <vector>

#include "../HasPtrAlias.hpp"

namespace Kyra {
template <typename ValueType>
struct Variable;
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