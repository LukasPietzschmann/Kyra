#pragma once

#include <string>
#include <utility>

namespace LibSlg {
class MessageException : std::exception {
public:
	MessageException() = delete;
	const char* what() const noexcept override { return m_message.c_str(); }

protected:
	explicit MessageException(std::string message) : m_message(std::move(message)) {}
	std::string m_message;
};

class LexerException : public MessageException {
public:
	explicit LexerException(const std::string& message) : MessageException(message) {}
};

class ParserException : public MessageException {
public:
	explicit ParserException(const std::string& message, bool unfinished = false) :
		MessageException(message), m_unfinished(unfinished) {}
	bool isUnfinished() const { return m_unfinished; }

private:
	const bool m_unfinished;
};

class RuntimeException : public MessageException {
public:
	explicit RuntimeException(const std::string& message) : MessageException(message) {}
};

class TypingException : public MessageException {
public:
	TypingException() = delete;

protected:
	explicit TypingException(const std::string& message) : MessageException(message) {}
};

class WrongTypeException : public TypingException {
public:
	WrongTypeException(const std::string& expected, const std::string& provided) :
		TypingException("Expected type " + expected + " does not equal provided type " + provided) {}
};

class UndefinedTypeException : public TypingException {
public:
	explicit UndefinedTypeException(const std::string& type) : TypingException("Undefined type " + type) {}
};

class AlreadyDefinedTypeException : public TypingException {
public:
	explicit AlreadyDefinedTypeException(const std::string& type) : TypingException("Already defined type " + type) {}
};

class UndefinedMemberException : public TypingException {
public:
	UndefinedMemberException(const std::string& owner, const std::string& member) :
		TypingException("Undefined member " + member + " in " + owner) {}
};

class AlreadyDefinedMemberException : public TypingException {
public:
	AlreadyDefinedMemberException(const std::string& owner, const std::string& member) :
		TypingException("Already defined member " + member + " in " + owner) {}
};

class UndefinedVariableException : public TypingException {
public:
	explicit UndefinedVariableException(const std::string& var) : TypingException("Undefined variable " + var) {}
};

class AlreadyDefinedVariableException : public TypingException {
public:
	explicit AlreadyDefinedVariableException(const std::string& var) :
		TypingException("Already defined variable " + var) {}
};

class AssignmentToConstException : public TypingException {
public:
	explicit AssignmentToConstException(const std::string& var) :
		TypingException("Constant variable " + var + " can not be mutated") {}
};

class ArityException : public TypingException {
public:
	ArityException(unsigned int expected, unsigned int provided, const std::string& name) :
		TypingException("The function " + name + " requires " + std::to_string(expected) +
						" arguments, but you provided " + std::to_string(provided)) {}
};

class InvalidReturnException : public TypingException {
public:
	InvalidReturnException() : TypingException("You can only return from a function context") {}
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