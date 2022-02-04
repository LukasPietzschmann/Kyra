#include "TypingErrors.hpp"

#include <sstream>
#include <string>
#include <utility>

namespace Kyra {
TypingError::TypingError(const Position& position, std::string message) :
	m_position(position), m_message(std::move(message)) {}

std::string TypingError::getCause() const {
	std::stringstream ss;
	ss << m_message << " [" << m_position.start << " - " << m_position.end << "]";
	return ss.str();
}

WrongTypeError::WrongTypeError(const Position& position, const std::string& expected, const std::string& provided) :
	TypingError(position, "Expected type " + expected + " does not equal provided type " + provided) {}

UnsupportedOperator::UnsupportedOperator(const Position& position,
		const std::string& unaryOper,
		const std::string& type) :
	TypingError(position, "Unsupported operator " + unaryOper + " on type " + type) {}

UnsupportedOperator::UnsupportedOperator(const Position& position,
		const std::string& lhsType,
		const std::string& binaryOper,
		const std::string& rhsType) :
	TypingError(position,
			"Unsupported operator " + binaryOper + " on left type " + lhsType + " with right type " + rhsType) {}

UndefinedTypeError::UndefinedTypeError(const Position& position, const std::string& type) :
	TypingError(position, "Undefined type " + type) {}

AlreadyDefinedTypeError::AlreadyDefinedTypeError(const Position& position, const std::string& type) :
	TypingError(position, "Already defined type " + type) {}

UndefinedMemberError::UndefinedMemberError(const Position& position,
		const std::string& owner,
		const std::string& member) :
	TypingError(position, "Undefined member " + member + " in " + owner) {}

AlreadyDefinedMemberError::AlreadyDefinedMemberError(const Position& position,
		const std::string& owner,
		const std::string& member) :
	TypingError(position, "Already defined member " + member + " in " + owner) {}

UndefinedVariableError::UndefinedVariableError(const Position& position, const std::string& var) :
	TypingError(position, "Undefined variable " + var) {}

AlreadyDefinedVariableError::AlreadyDefinedVariableError(const Position& position, const std::string& var) :
	TypingError(position, "Already defined variable " + var) {}

AssignmentToConstError::AssignmentToConstError(const Position& position, const std::string& var) :
	TypingError(position, "Constant variable " + var + " can not be mutated") {}

ArityError::ArityError(const Position& position,
		unsigned int expected,
		unsigned int provided,
		const std::string& name) :
	TypingError(position,
			"The function " + name + " requires " + std::to_string(expected) + " arguments, but you provided " +
					std::to_string(provided)) {}

InvalidReturnError::InvalidReturnError(const Position& position) :
	TypingError(position, "You can only return from a function context") {}
}