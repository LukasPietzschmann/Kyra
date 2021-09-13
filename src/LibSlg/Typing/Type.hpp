#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <utility>

#include "../Values/Function.hpp"
#include "../Values/Value.hpp"

namespace LibSlg {
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
	Type(std::string name, bool isMutable) : m_name(std::move(name)), m_isMutable(isMutable) {}
	virtual Type::Ptr knowsAbout(const std::string&) const { return nullptr; }

	bool isMutable() const { return m_isMutable; }
	const std::string& getName() const { return m_name; }

	virtual bool operator==(const Type::Ptr& other) const { return m_name == other->m_name; }
	virtual bool operator!=(const Type::Ptr& other) const { return !(*this == other); }

protected:
	std::string m_name;
	bool m_isMutable;
};
}