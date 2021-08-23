#pragma once

#include <unordered_map>
#include <utility>
#include "Exceptions.hpp"
#include "Value.hpp"

namespace LibSlg {
class Context {
public:
	typedef std::shared_ptr<Context> Ptr;
	typedef struct ContextValue {
		ContextValue(const Value::Ptr& value, bool isMutable) : value(value), isMutable(isMutable) {}
		Value::Ptr value;
		bool isMutable;
	} ContextValue;
	explicit Context(Ptr parent = nullptr) : m_parent(parent) {}

	const std::shared_ptr<Context>& getParent() const;
	ContextValue get(const std::string& name);
	void declare(const std::string& name, Value::Ptr value = Value::makePtr(), bool isMutable = true);
	void mutate(const std::string& name, Value::Ptr value);

	template <class... Args>
	static Context::Ptr makePtr(Args... args) {
		static_assert(std::is_constructible<Context, Args...>::value, "Cannot construct object in Context::makePtr");
		return std::make_shared<Context>(args...);
	}
private:
	const std::shared_ptr<Context> m_parent;
	std::unordered_map<std::string, ContextValue> m_variables;
};
}