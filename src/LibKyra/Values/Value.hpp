#pragma once

#include <cassert>
#include <memory>
#include <ostream>
#include <string>
#include <vector>

#include "../Exceptions.hpp"
#include "../HasPtrAlias.hpp"
#include "../Position.hpp"

namespace Kyra {
class Value : public HasPtrAlias<Value> {
public:
	struct NativeTypes {
		static const std::string Nothing;
		static const std::string Number;
		static const std::string Bool;
		static const std::string String;
		static const std::string Function;
	};

	virtual bool is_implicitly_true() const = 0;
	virtual std::string get_type() const = 0;
	virtual std::string to_string() const = 0;

	template <typename R>
	static std::shared_ptr<R> as(Value::Ptr value) {
		return std::dynamic_pointer_cast<R>(value);
	}

	virtual bool is_native() const { return true; }

	virtual bool operator==(const Value::Ptr&) const { return false; }
	virtual bool operator<(const Value::Ptr&) const { return false; }
	virtual bool operator>(const Value::Ptr&) const { return false; }
	virtual bool operator!=(const Value::Ptr& other) const { return !operator==(other); };
	virtual bool operator<=(const Value::Ptr& other) const { return operator<(other) || operator==(other); };
	virtual bool operator>=(const Value::Ptr& other) const { return operator>(other) || operator==(other); };
	// TODO: This needs to be done better!
	virtual Value::Ptr operator+(const Value::Ptr&) const { assert(false); }
	virtual Value::Ptr operator-(const Value::Ptr&) const { assert(false); }
	virtual Value::Ptr operator*(const Value::Ptr&) const { assert(false); }
	virtual Value::Ptr operator/(const Value::Ptr&) const { assert(false); }

protected:
	virtual ~Value() = default;
};
}