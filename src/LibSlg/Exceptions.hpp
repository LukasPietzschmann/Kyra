#pragma once

#include <string>
#include <utility>

namespace LibSlg {
class MessageException : std::exception {
public:
	explicit MessageException(const std::string& message) : m_message(message) {}
	virtual const char* what() const noexcept override { return m_message.c_str(); }

protected:
	std::string m_message;
};

class LexerException : public MessageException {
public:
	explicit LexerException(std::string message) : MessageException(message) {}
};

class ParserException : public MessageException {
public:
	explicit ParserException(std::string message, bool unfinished = false) :
		MessageException(message), m_unfinished(unfinished) {}
	bool isUnfinished() const { return m_unfinished; }

private:
	const bool m_unfinished;
};

class RuntimeException : public MessageException {
public:
	explicit RuntimeException(std::string message) : MessageException(message) {}
};

class Value;

class ReturnException : std::exception {
public:
	ReturnException(const std::shared_ptr<Value>& returnVal) : m_returnVal(returnVal) {}
	const std::shared_ptr<Value>& getReturnVal() const { return m_returnVal; }

private:
	std::shared_ptr<Value> m_returnVal;
};
}