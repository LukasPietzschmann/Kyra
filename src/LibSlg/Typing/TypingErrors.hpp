#pragma once

#include <sstream>
#include <string>
#include <utility>

#include "../Position.hpp"

namespace Slanguage {
class TypingError {
public:
	TypingError(const Position& position, std::string message) : m_position(position), m_message(std::move(message)) {}

	std::string getCause() const {
		std::stringstream ss;
		ss << m_message << " [" << m_position.start << " - " << m_position.end << "]";
		return ss.str();
	}

private:
	Position m_position;
	std::string m_message;
};

class WrongTypeError : public TypingError {
public:
	WrongTypeError(const Position& position, const std::string& expected, const std::string& provided) :
		TypingError(position, "Expected type " + expected + " does not equal provided type " + provided) {}
};

class UnsupportedOperator : public TypingError {
public:
	UnsupportedOperator(const Position& position, const std::string& unaryOper, const std::string& type) :
		TypingError(position, "Unsupported operator " + unaryOper + " on type " + type) {}

	UnsupportedOperator(const Position& position,
			const std::string& lhsType,
			const std::string& binaryOper,
			const std::string& rhsType) :
		TypingError(position,
				"Unsupported operator " + binaryOper + " on left type " + lhsType + " with right type " + rhsType) {}
};

class UndefinedTypeError : public TypingError {
public:
	explicit UndefinedTypeError(const Position& position, const std::string& type) :
		TypingError(position, "Undefined type " + type) {}
};

class AlreadyDefinedTypeError : public TypingError {
public:
	explicit AlreadyDefinedTypeError(const Position& position, const std::string& type) :
		TypingError(position, "Already defined type " + type) {}
};

class UndefinedMemberError : public TypingError {
public:
	UndefinedMemberError(const Position& position, const std::string& owner, const std::string& member) :
		TypingError(position, "Undefined member " + member + " in " + owner) {}
};

class AlreadyDefinedMemberError : public TypingError {
public:
	AlreadyDefinedMemberError(const Position& position, const std::string& owner, const std::string& member) :
		TypingError(position, "Already defined member " + member + " in " + owner) {}
};

class UndefinedVariableError : public TypingError {
public:
	explicit UndefinedVariableError(const Position& position, const std::string& var) :
		TypingError(position, "Undefined variable " + var) {}
};

class AlreadyDefinedVariableError : public TypingError {
public:
	explicit AlreadyDefinedVariableError(const Position& position, const std::string& var) :
		TypingError(position, "Already defined variable " + var) {}
};

class AssignmentToConstError : public TypingError {
public:
	explicit AssignmentToConstError(const Position& position, const std::string& var) :
		TypingError(position, "Constant variable " + var + " can not be mutated") {}
};

class ArityError : public TypingError {
public:
	ArityError(const Position& position, unsigned int expected, unsigned int provided, const std::string& name) :
		TypingError(position,
				"The function " + name + " requires " + std::to_string(expected) + " arguments, but you provided " +
						std::to_string(provided)) {}
};

class InvalidReturnError : public TypingError {
public:
	explicit InvalidReturnError(const Position& position) :
		TypingError(position, "You can only return from a function context") {}
};
}