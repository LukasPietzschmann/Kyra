#pragma once

#include <unordered_map>
#include <utility>
#include "Exceptions.hpp"
#include "Value.hpp"

namespace LibSlg {
class Context {
public:
	typedef std::shared_ptr<Context> Ptr;
	explicit Context(Ptr parent = nullptr) : m_parent(parent) {}

	const std::shared_ptr<Context>& getParent() const;
	Value::Ptr get(const std::string& name);
	void declare(const std::string& name, Value::Ptr value = Value::makePtr());
	void mutate(const std::string& name, Value::Ptr value);

	template <class... Args>
	static Context::Ptr makePtr(Args... args) {
		static_assert(std::is_constructible<Context, Args...>::value, "Cannot construct object in Context::makePtr");
		return std::make_shared<Context>(args...);
	}
private:
	const std::shared_ptr<Context> m_parent;
	std::unordered_map<std::string, Value::Ptr> m_variables;
};
}