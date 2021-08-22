#include "Value.hpp"

namespace LibSlg {
Value::~Value() {
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

Value::Value(const Value& other) : m_type(other.m_type) {
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

bool Value::isTrue() {
	if(m_type == BOOL)
		return m_boolVal;
	return m_type != NOTHING;
}

std::shared_ptr<Function> Value::function() const {
	if(m_type != FUNCTION)
		throw BadEntryException();
	return m_functionVal;
}

std::shared_ptr<Context> Value::object() const {
	if(m_type != OBJECT)
		throw BadEntryException();
	return m_objectVal;
}

std::string Value::string() const {
	if(m_type != STRING)
		throw BadEntryException();
	return m_stringVal;
}

bool Value::boolean() const {
	if(m_type != BOOL)
		throw BadEntryException();
	return m_boolVal;
}

int Value::number() const {
	if(m_type != NUMBER)
		throw BadEntryException();
	return m_intVal;
}

std::ostream& operator<<(std::ostream& os, const Value& value) {
	switch(value.m_type) {
		case Value::NOTHING: os << "nothing";
			break;
		case Value::BOOL: os << (value.m_boolVal ? "true" : "false");
			break;
		case Value::NUMBER: os << value.m_intVal;
			break;
		case Value::STRING: os << value.m_stringVal;
			break;
		case Value::OBJECT: os << "[object]";
			break;
		case Value::FUNCTION: os << "[function]";
			break;
	}
	return os;
}

bool Value::operator==(const Value& other) const {
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
bool Value::operator!=(const Value& other) const { return !(other == *this); }

bool Value::operator<(const Value& other) const {
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

bool Value::operator>(const Value& other) const {
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

bool Value::operator<=(const Value& other) const {
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

bool Value::operator>=(const Value& other) const {
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

Value Value::operator+(const Value& other) const {
	if(!(m_type == NUMBER && other.m_type == NUMBER) && !(m_type == STRING && other.m_type == STRING))
		throw BadEntryException();
	if(m_type == NUMBER)
		return Value(m_intVal + other.m_intVal);
	if(m_type == STRING)
		return Value(m_stringVal + other.m_stringVal);
	assert(false);
}

Value Value::operator-(const Value& other) const {
	if(m_type != NUMBER || other.m_type != NUMBER)
		throw BadEntryException();
	return Value(m_intVal - other.m_intVal);
}

Value Value::operator/(const Value& other) const {
	if(m_type != NUMBER || other.m_type != NUMBER)
		throw BadEntryException();
	return Value(m_intVal / other.m_intVal);
}

Value Value::operator*(const Value& other) const {
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
}