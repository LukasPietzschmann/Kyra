#pragma once

#include <string>
#include <utility>

namespace LibSlg {
class MessageException : std::exception {
public:
	explicit MessageException(std::string message) : m_message(std::move(message)) {}
	const char* what() const noexcept override { return m_message.c_str(); }

protected:
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