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

	~Value() {
		switch(m_type) {
			case NOTHING:
			case NUMBER:
			case OBJECT:
			case FUNCTION:
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
			case OBJECT: m_objectVal = other.m_objectVal;
				break;
			case FUNCTION: m_functionVal = other.m_functionVal;
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

	std::shared_ptr<Context> object() const {
		if(m_type != OBJECT)
			throw BadEntryException();
		return m_objectVal;
	}

	std::shared_ptr<Function> function() const {
		if(m_type != FUNCTION)
			throw BadEntryException();
		return m_functionVal;
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
			case OBJECT: os << "[object]";
				break;
			case FUNCTION: os << "[function]";
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
			case OBJECT: return m_objectVal == other.m_objectVal;
			case FUNCTION: return m_functionVal == other.m_functionVal;
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
			case OBJECT: return false;
			case FUNCTION: return false;
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
			case OBJECT: return false;
			case FUNCTION: return false;
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
			case OBJECT: return *this == other;
			case FUNCTION: return *this == other;
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
			case OBJECT: return *this == other;
			case FUNCTION: return *this == other;
		}
	}

	Value operator+(const Value& other) const {
		if(!(m_type == NUMBER && other.m_type == NUMBER) && !(m_type == STRING && other.m_type == STRING))
			throw BadEntryException();
		if(m_type == NUMBER)
			return Value(m_intVal + other.m_intVal);
		if(m_type == STRING)
			return Value(m_stringVal + other.m_stringVal);
		assert(false);
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
		std::shared_ptr<Context> m_objectVal; //FIXME Context::Ptr not accessible as it's a nested non forward declarable typedef.
		std::shared_ptr<Function> m_functionVal; //FIXME Function::Ptr not accessible as it's a nested non forward declarable typedef.
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