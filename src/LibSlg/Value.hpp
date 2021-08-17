#pragma once

#include <memory>
#include <ostream>

namespace LibSlg {
class Value {
public:
	class BadEntryException : std::exception {};

	typedef enum { NOTHING, NUMBER, BOOL, STRING } Type;
	typedef std::shared_ptr<Value> Ptr;

	Value() : m_type(NOTHING) {};
	explicit Value(int value) : m_type(NUMBER), m_intVal(value) {}
	explicit Value(bool value) : m_type(BOOL), m_boolVal(value) {}
	explicit Value(std::string value) : m_type(STRING), m_stringVal(std::move(value)) {}

	~Value() {
		switch(m_type) {
			case NOTHING:
			case NUMBER:
			case BOOL: break;
			case STRING: m_stringVal.~basic_string();
				break;
		}
	}

	Value(const Value& other) : m_type(other.m_type) {
		switch(m_type) {
			case NOTHING: break;
			case NUMBER: m_intVal = other.m_intVal;
				break;
			case BOOL: m_boolVal = other.m_boolVal;
				break;
			case STRING: m_stringVal = other.m_stringVal;
				break;
		}
	}

	Type getType() const { return m_type; }

	int number() const {
		if(m_type != NUMBER)
			throw BadEntryException();
		return m_intVal;
	}

	bool boolean() const {
		if(m_type != BOOL)
			throw BadEntryException();
		return m_boolVal;
	}

	std::string string() const {
		if(m_type != STRING)
			throw BadEntryException();
		return m_stringVal;
	}

	friend std::ostream& operator<<(std::ostream& os, const Value& value) {
		switch(value.m_type) {
			case NOTHING: os << "NOTHING";
				break;
			case BOOL: os << value.m_boolVal;
				break;
			case NUMBER: os << value.m_intVal;
				break;
			case STRING: os << value.m_stringVal;
				break;
		}
		return os;
	}
private:
	union {
		bool m_boolVal {};
		int m_intVal;
		std::string m_stringVal;
	};
	Type m_type;
};

class ValueTypeName {
public:
	static std::string getFor(Value::Type type) {
		const std::string names[] = {"NOTHING", "NUMBER", "BOOL", "STRING"};
		return names[static_cast<std::underlying_type<Value::Type>::type>(type)];
	}
};
}