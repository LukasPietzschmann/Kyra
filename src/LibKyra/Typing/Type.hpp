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
	explicit Type(std::string name);
	virtual ~Type() = default;

	virtual std::optional<Variable<Type::Ptr>> knows_about(const std::string&) const;
	virtual bool can_be_called_with(const std::vector<Type::Ptr>&) const;
	virtual bool is_applicable_for_declaration() const;
	virtual bool can_be_assigned_to(const Type::Ptr& assignee) const;
	virtual bool is_function() const;

	const std::string& get_name() const;

	virtual bool operator==(const Type::Ptr& other) const;
	virtual bool operator!=(const Type::Ptr& other) const;

protected:
	std::string m_name;
};
}