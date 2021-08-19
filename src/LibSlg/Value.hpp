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

	bool isTrue() {
		if(m_type == BOOL)
			return m_boolVal;
		return m_type != NOTHING;
	}

	template <class... Args>
	static Value::Ptr makePtr(Args... args) {
		static_assert(std::is_constructible<Value, Args...>::value, "Cannot construct Value in Value::makePtr");
		return std::make_shared<Value>(args...);
	}

	friend std::ostream& operator<<(std::ostream& os, const Value& value) {
		switch(value.m_type) {
			case NOTHING: os << "nothing";
				break;
			case BOOL: os << (value.m_boolVal ? "true" : "false");
				break;
			case NUMBER: os << value.m_intVal;
				break;
			case STRING: os << value.m_stringVal;
				break;
		}
		return os;
	}

	bool operator==(const Value& other) const {
		if(m_type != other.m_type)
			return false;
		switch(m_type) {
			case NOTHING: return true;
			case BOOL: return m_boolVal == other.m_boolVal;
			case NUMBER: return m_intVal == other.m_intVal;
			case STRING: return m_stringVal == other.m_stringVal;
		}
	}

	bool operator!=(const Value& other) const { return !(other == *this); }

	bool operator<(const Value& other) const {
		if(m_type != other.m_type)
			return false;
		switch(m_type) {
			case BOOL: return !m_boolVal && other.m_boolVal;
			case NOTHING: return false;
			case NUMBER: return m_intVal < other.m_intVal;
			case STRING: return m_stringVal < other.m_stringVal;
		}
	}

	bool operator>(const Value& other) const {
		if(m_type != other.m_type)
			return false;
		switch(m_type) {
			case BOOL: return m_boolVal && !other.m_boolVal;
			case NOTHING: return false;
			case NUMBER: return m_intVal > other.m_intVal;
			case STRING: return m_stringVal > other.m_stringVal;
		}
	}

	bool operator<=(const Value& other) const {
		if(m_type != other.m_type)
			return false;
		switch(m_type) {
			case BOOL: return !(m_boolVal && !other.m_boolVal);
			case NOTHING: return true;
			case NUMBER: return m_intVal <= other.m_intVal;
			case STRING: return m_stringVal <= other.m_stringVal;
		}
	}

	bool operator>=(const Value& other) const {
		if(m_type != other.m_type)
			return false;
		switch(m_type) {
			case BOOL: return !(!m_boolVal && other.m_boolVal);
			case NOTHING: return true;
			case NUMBER: return m_intVal >= other.m_intVal;
			case STRING: return m_stringVal >= other.m_stringVal;
		}
	}

	Value operator+(const Value& other) const {
		if(m_type != NUMBER || other.m_type != NUMBER)
			throw BadEntryException();
		return Value(m_intVal + other.m_intVal);
	}

	Value operator-(const Value& other) const {
		if(m_type != NUMBER || other.m_type != NUMBER)
			throw BadEntryException();
		return Value(m_intVal - other.m_intVal);
	}

	Value operator/(const Value& other) const {
		if(m_type != NUMBER || other.m_type != NUMBER)
			throw BadEntryException();
		return Value(m_intVal / other.m_intVal);
	}

	Value operator*(const Value& other) const {
		if(m_type == STRING && other.m_type == NUMBER) {
			int mult = other.m_intVal;
			std::string out;
			while(mult-- > 0)
				out += m_stringVal;
			return Value(out);
		}
		if(m_type != NUMBER || other.m_type != NUMBER)
			throw BadEntryException();
		return Value(m_intVal * other.m_intVal);
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