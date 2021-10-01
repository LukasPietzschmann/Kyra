#pragma once

#include <unordered_map>
#include <utility>
#include <vector>

#include "Exceptions.hpp"
#include "Values/Klass.hpp"
#include "Values/Nothing.hpp"

namespace LibSlg {
class Context {
public:
	using Ptr = std::shared_ptr<Context>;
	struct ContextValue {
		ContextValue(Value::Ptr value, bool isMutable) : value(std::move(value)), isMutable(isMutable) {}
		Value::Ptr value;
		bool isMutable;
	};
	explicit Context(Ptr parent = nullptr) : m_parent(std::move(std::move(parent))) {}

	const std::shared_ptr<Context>& getParent() const;
	ContextValue getVar(const std::string& name) const;
	const Klass& getCustomType(const Value::Type& type) const;
	void declareVar(const std::string& name, const Value::Ptr& value, bool isMutable = true);
	void mutate(const std::string& name, Value::Ptr value);
	void declareType(const Value::Type& type, const Klass& klass);

	template <class... Args>
	static Context::Ptr makePtr(Args... args) {
		static_assert(std::is_constructible_v<Context, Args...>, "Cannot construct object in Context::makePtr");
		return std::make_shared<Context>(args...);
	}

private:
	const std::shared_ptr<Context> m_parent;
	std::unordered_map<std::string, ContextValue> m_variables;
	std::vector<Value::Type> m_nativeTypes{Value::NativeTypes::Nothing,
			Value::NativeTypes::Number,
			Value::NativeTypes::Bool,
			Value::NativeTypes::String,
			Value::NativeTypes::Function};
	std::unordered_map<Value::Type, Klass> m_customTypes;
};
}