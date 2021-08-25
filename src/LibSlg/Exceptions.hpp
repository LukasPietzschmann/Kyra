#pragma once

#include <string>
#include <utility>

namespace LibSlg {
class LexerException : std::exception {
public:
	explicit LexerException(std::string message) : m_message(std::move(message)) {}
	const char* what() const noexcept override { return m_message.c_str(); }
private:
	std::string m_message;
};

class ParserException : std::exception {
public:
	explicit ParserException(std::string message, bool unfinished = false) :
			m_message(std::move(message)), m_unfinished(unfinished) {}
	const char* what() const noexcept override { return m_message.c_str(); }
	bool isUnfinished() const { return m_unfinished; }
private:
	std::string m_message;
	const bool m_unfinished;
};

class RuntimeException : std::exception {
public:
	explicit RuntimeException(std::string message) : m_message(std::move(message)) {}
	const char* what() const noexcept override { return m_message.c_str(); }
private:
	std::string m_message;
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