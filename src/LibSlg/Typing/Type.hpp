#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <utility>

#include "../Values/Function.hpp"
#include "../Values/Value.hpp"

namespace LibSlg {
class Type;
struct Variable {
	Variable(const std::shared_ptr<Type>& type, bool isMutable) : type(type), isMutable(isMutable) {}
	std::shared_ptr<Type> type;
	bool isMutable;
};

class Type {
public:
	using Ptr = std::shared_ptr<Type>;
	explicit Type(std::string name) : m_name(std::move(name)) {}
	virtual ~Type() = default;

	template <typename T, class... Args>
	static Type::Ptr makePtr(Args... args) {
		static_assert(std::is_constructible_v<T, Args...>, "Cannot construct object in Type::makePtr");
		return std::make_shared<T>(args...);
	}

	virtual std::optional<Variable> knowsAbout(const std::string&) const { return {}; }
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