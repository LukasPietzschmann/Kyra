#pragma once

#include <sstream>
#include <string>
#include <utility>

#include "Position.hpp"

namespace LibSlg {
class MessageException : std::exception {
public:
	MessageException() = delete;
	const char* what() const noexcept override { return m_computedMessage.c_str(); }

protected:
	MessageException(const Position& position, std::string message) :
		m_position(position), m_message(std::move(message)) {
		std::stringstream ss;
		ss << m_message << " [" << m_position.start << " - " << m_position.end << "]";
		m_computedMessage = ss.str();
	}
	Position m_position;
	std::string m_message;

private:
	std::string m_computedMessage;
};

class LexerException : public MessageException {
public:
	LexerException(const Position& position, const std::string& message) : MessageException(position, message) {}
};

class ParserException : public MessageException {
public:
	ParserException(const Position& position, const std::string& message, bool unfinished = false) :
		MessageException(position, message), m_unfinished(unfinished) {}
	bool isUnfinished() const { return m_unfinished; }

private:
	const bool m_unfinished;
};

class RuntimeException : public MessageException {
public:
	explicit RuntimeException(const Position& position, const std::string& message) :
		MessageException(position, message) {}
};

class TypingException : public MessageException {
public:
	TypingException() = delete;

protected:
	explicit TypingException(const Position& position, const std::string& message) :
		MessageException(position, message) {}
};

class WrongTypeException : public TypingException {
public:
	WrongTypeException(const Position& position, const std::string& expected, const std::string& provided) :
		TypingException(position, "Expected type " + expected + " does not equal provided type " + provided) {}
};

class UndefinedTypeException : public TypingException {
public:
	explicit UndefinedTypeException(const Position& position, const std::string& type) :
		TypingException(position, "Undefined type " + type) {}
};

class AlreadyDefinedTypeException : public TypingException {
public:
	explicit AlreadyDefinedTypeException(const Position& position, const std::string& type) :
		TypingException(position, "Already defined type " + type) {}
};

class UndefinedMemberException : public TypingException {
public:
	UndefinedMemberException(const Position& position, const std::string& owner, const std::string& member) :
		TypingException(position, "Undefined member " + member + " in " + owner) {}
};

class AlreadyDefinedMemberException : public TypingException {
public:
	AlreadyDefinedMemberException(const Position& position, const std::string& owner, const std::string& member) :
		TypingException(position, "Already defined member " + member + " in " + owner) {}
};

class UndefinedVariableException : public TypingException {
public:
	explicit UndefinedVariableException(const Position& position, const std::string& var) :
		TypingException(position, "Undefined variable " + var) {}
};

class AlreadyDefinedVariableException : public TypingException {
public:
	explicit AlreadyDefinedVariableException(const Position& position, const std::string& var) :
		TypingException(position, "Already defined variable " + var) {}
};

class AssignmentToConstException : public TypingException {
public:
	explicit AssignmentToConstException(const Position& position, const std::string& var) :
		TypingException(position, "Constant variable " + var + " can not be mutated") {}
};

class ArityException : public TypingException {
public:
	ArityException(const Position& position, unsigned int expected, unsigned int provided, const std::string& name) :
		TypingException(position, "The function " + name + " requires " + std::to_string(expected) +
										  " arguments, but you provided " + std::to_string(provided)) {}
};

class InvalidReturnException : public TypingException {
public:
	InvalidReturnException(const Position& position) :
		TypingException(position, "You can only return from a function context") {}
};

class Value;

class ReturnException : std::exception {
public:
	explicit ReturnException(std::shared_ptr<Value> returnVal) : m_returnVal(std::move(returnVal)) {}
	const std::shared_ptr<Value>& getReturnVal() const { return m_returnVal; }

private:
	std::shared_ptr<Value> m_returnVal;
};
}