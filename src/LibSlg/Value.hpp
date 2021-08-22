#pragma once

#include <memory>
#include <ostream>

namespace LibSlg {
class Context;
class Function;

class Value {
public:
	class BadEntryException : std::exception {};

	typedef enum { NOTHING, NUMBER, BOOL, STRING, OBJECT, FUNCTION } Type;
	typedef std::shared_ptr<Value> Ptr;

	Value() : m_type(NOTHING) {};
	explicit Value(int value) : m_intVal(value), m_type(NUMBER) {}
	explicit Value(bool value) : m_boolVal(value), m_type(BOOL) {}
	explicit Value(std::string value) : m_stringVal(std::move(value)), m_type(STRING) {}
	explicit Value(std::shared_ptr<Context> value) : m_objectVal(value), m_type(OBJECT) {}
	explicit Value(std::shared_ptr<Function> value) : m_functionVal(value), m_type(FUNCTION) {}

	~Value();
	Value(const Value& other);

	Type getType() const { return m_type; }
	int number() const;
	bool boolean() const;
	std::string string() const;
	std::shared_ptr<Context> object() const;
	std::shared_ptr<Function> function() const;
	bool isTrue();

	template <class... Args>
	static Value::Ptr makePtr(Args... args) {
		static_assert(std::is_constructible<Value, Args...>::value, "Cannot construct Value in Value::makePtr");
		return std::make_shared<Value>(args...);
	}

	friend std::ostream& operator<<(std::ostream& os, const Value& value);
	bool operator==(const Value& other) const;
	bool operator!=(const Value& other) const;
	bool operator<(const Value& other) const;
	bool operator>(const Value& other) const;
	bool operator<=(const Value& other) const;
	bool operator>=(const Value& other) const;
	Value operator+(const Value& other) const;
	Value operator-(const Value& other) const;
	Value operator/(const Value& other) const;
	Value operator*(const Value& other) const;
private:
	union {
		bool m_boolVal {};
		int m_intVal;
		std::string m_stringVal;
		std::shared_ptr<Context> m_objectVal;
		std::shared_ptr<Function> m_functionVal;
	};
	Type m_type;
};

class ValueTypeName {
public:
	static std::string getFor(Value::Type type) {
		const std::string names[] = {"NOTHING", "NUMBER", "BOOL", "STRING", "OBJECT", "FUNCTION"};
		return names[static_cast<std::underlying_type<Value::Type>::type>(type)];
	}
};
}