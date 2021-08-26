#pragma once

#include <unordered_map>
#include <utility>
#include <vector>
#include "Exceptions.hpp"
#include "Values/Nothing.hpp"

namespace LibSlg {
class Context {
public:
	typedef std::shared_ptr<Context> Ptr;
	typedef struct ContextValue {
		ContextValue(const Value::Ptr& value, Value::Type type, bool isMutable) :
				value(value), type(type), isMutable(isMutable) {}
		Value::Ptr value;
		Value::Type type;
		bool isMutable;
	} ContextValue;
	explicit Context(Ptr parent = nullptr) : m_parent(parent) {}

	const std::shared_ptr<Context>& getParent() const;
	ContextValue get(const std::string& name);
	void declare(const std::string& name, Value::Ptr value, Value::Type type, bool isMutable = true);
	void mutate(const std::string& name, Value::Ptr value);
	bool isTypeKnown(const Value::Type& type);
	bool declareType(const Value::Type& type);

	template <class... Args>
	static Context::Ptr makePtr(Args... args) {
		static_assert(std::is_constructible<Context, Args...>::value, "Cannot construct object in Context::makePtr");
		return std::make_shared<Context>(args...);
	}
private:
	const std::shared_ptr<Context> m_parent;
	std::unordered_map<std::string, ContextValue> m_variables;
	std::vector<Value::Type> m_types {Value::NativeTypes::Nothing, Value::NativeTypes::Number, Value::NativeTypes::Bool,
			Value::NativeTypes::String, Value::NativeTypes::Object, Value::NativeTypes::Function};
};
}