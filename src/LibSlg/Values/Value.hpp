#pragma once

#include <memory>
#include <ostream>
#include <string>
#include <vector>

#include "../Exceptions.hpp"
#include "../HasPtrAlias.hpp"
#include "../Position.hpp"

namespace Slanguage {
class Value : public HasPtrAlias<Value> {
public:
	struct NativeTypes {
		static const std::string Nothing;
		static const std::string Number;
		static const std::string Bool;
		static const std::string String;
		static const std::string Function;
		static const std::vector<std::string> All;
	};

	virtual bool isImplicitlyTrue() const = 0;
	virtual std::string getType() const = 0;
	virtual std::string toString() const = 0;

	virtual bool hasCorrectTypeForAssignment(const std::string& type) const { return getType() == type; };

	template <typename R>
	static std::shared_ptr<R> as(Value::Ptr value) {
		return std::dynamic_pointer_cast<R>(value);
	}

	virtual bool operator==(const Value::Ptr&) const = 0;
	virtual bool operator<(const Value::Ptr&) const = 0;
	virtual bool operator>(const Value::Ptr&) const = 0;

	virtual bool operator!=(const Value::Ptr& other) const { return !operator==(other); };

	virtual bool operator<=(const Value::Ptr& other) const { return operator<(other) || operator==(other); };

	virtual bool operator>=(const Value::Ptr& other) const { return operator>(other) || operator==(other); };

	virtual Value::Ptr operator+(const Value::Ptr&) const {
		throw ParserException(Position(0, 0, 0, 0), "Operator + can't be used here");
	}

	virtual Value::Ptr operator-(const Value::Ptr&) const {
		throw ParserException(Position(0, 0, 0, 0), "Operator - can't be used here");
	}

	virtual Value::Ptr operator*(const Value::Ptr&) const {
		throw ParserException(Position(0, 0, 0, 0), "Operator * can't be used here");
	}

	virtual Value::Ptr operator/(const Value::Ptr&) const {
		throw ParserException(Position(0, 0, 0, 0), "Operator / can't be used here");
	}

protected:
	virtual ~Value() = default;
};
}