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
	typedef std::shared_ptr<Type> Ptr;
	virtual ~Type() = default;

	template <typename T, class... Args>
	static Type::Ptr makePtr(Args... args) {
		static_assert(std::is_constructible<T, Args...>::value, "Cannot construct object in Type::makePtr");
		return std::make_shared<T>(args...);
	}

public:
	Type(std::string name) : m_name(std::move(name)) {}
	virtual std::optional<Variable> knowsAbout(const std::string&) const { return {}; }
	virtual bool canBeCalledWith(std::vector<Type::Ptr>) const { return false; };

	const std::string& getName() const { return m_name; }

	virtual bool operator==(const Type::Ptr& other) const { return m_name == other->m_name; }
	virtual bool operator!=(const Type::Ptr& other) const { return !(*this == other); }

protected:
	std::string m_name;
};
}