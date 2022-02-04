#pragma once

#include "../Position.hpp"

namespace Kyra {
class TypingError {
public:
	TypingError(const Position& position, std::string message);

	std::string getCause() const;

private:
	Position m_position;
	std::string m_message;
};

class WrongTypeError : public TypingError {
public:
	WrongTypeError(const Position& position, const std::string& expected, const std::string& provided);
};

class UnsupportedOperator : public TypingError {
public:
	UnsupportedOperator(const Position& position, const std::string& unaryOper, const std::string& type);

	UnsupportedOperator(const Position& position,
			const std::string& lhsType,
			const std::string& binaryOper,
			const std::string& rhsType);
};

class UndefinedTypeError : public TypingError {
public:
	explicit UndefinedTypeError(const Position& position, const std::string& type);
};

class AlreadyDefinedTypeError : public TypingError {
public:
	explicit AlreadyDefinedTypeError(const Position& position, const std::string& type);
};

class UndefinedMemberError : public TypingError {
public:
	UndefinedMemberError(const Position& position, const std::string& owner, const std::string& member);
};

class AlreadyDefinedMemberError : public TypingError {
public:
	AlreadyDefinedMemberError(const Position& position, const std::string& owner, const std::string& member);
};

class UndefinedVariableError : public TypingError {
public:
	explicit UndefinedVariableError(const Position& position, const std::string& var);
};

class AlreadyDefinedVariableError : public TypingError {
public:
	explicit AlreadyDefinedVariableError(const Position& position, const std::string& var);
};

class AssignmentToConstError : public TypingError {
public:
	explicit AssignmentToConstError(const Position& position, const std::string& var);
};

class ArityError : public TypingError {
public:
	ArityError(const Position& position, unsigned int expected, unsigned int provided, const std::string& name);
};

class InvalidReturnError : public TypingError {
public:
	explicit InvalidReturnError(const Position& position);
};
}