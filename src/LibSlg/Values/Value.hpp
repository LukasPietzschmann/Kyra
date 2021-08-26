#pragma once

#include <memory>
#include <ostream>
#include "iostream"
#include "Forward.hpp"
#include "../Exceptions.hpp"

namespace LibSlg {
class Value {
public:
	typedef std::shared_ptr<Value> Ptr;
	typedef std::string Type;
	typedef struct NativeType {
		static const Type Nothing;
		static const Type Number;
		static const Type Bool;
		static const Type String;
		static const Type Function;
	} NativeTypes;

	virtual bool isImplicitlyTrue() const = 0;
	virtual Type getType() const = 0;
	virtual std::string toString() const = 0;

	template <typename R>
	static std::shared_ptr<R> as(Value::Ptr value) {
		return std::dynamic_pointer_cast<R>(value);
	}

	template <typename T, class... Args>
	static Value::Ptr makePtr(Args... args) {
		static_assert(std::is_constructible<T, Args...>::value, "Cannot construct Value in Value::makePtr");
		return std::make_shared<T>(args...);
	}

	virtual bool operator==(const Value::Ptr&) const = 0;
	virtual bool operator<(const Value::Ptr&) const = 0;
	virtual bool operator>(const Value::Ptr&) const = 0;

	virtual bool operator!=(const Value::Ptr& other) const {
		return !operator==(other);
	};

	virtual bool operator<=(const Value::Ptr& other) const {
		return operator<(other) || operator==(other);
	};

	virtual bool operator>=(const Value::Ptr& other) const {
		return operator>(other) || operator==(other);
	};

	virtual Value::Ptr operator+(const Value::Ptr&) const {
		throw ParserException("Operator + can't be used here");
	}

	virtual Value::Ptr operator-(const Value::Ptr&) const {
		throw ParserException("Operator - can't be used here");
	}

	virtual Value::Ptr operator*(const Value::Ptr&) const {
		throw ParserException("Operator * can't be used here");
	}

	virtual Value::Ptr operator/(const Value::Ptr&) const {
		throw ParserException("Operator / can't be used here");
	}
protected:
	virtual ~Value() {};
};
}