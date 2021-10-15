#pragma once

#include <sstream>
#include <string>
#include <utility>

#include "Position.hpp"

namespace Slanguage {
class MessageException : public std::exception {
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

private:
	Position m_position;
	std::string m_message;
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

class TypingException : public MessageException {
public:
	explicit TypingException(const std::string& message) : MessageException(Position(0, 0, 0, 0), message) {}
};

class Value;

class ReturnException : public std::exception {
public:
	explicit ReturnException(std::shared_ptr<Value> returnVal) : m_returnVal(std::move(returnVal)) {}
	const std::shared_ptr<Value>& getReturnVal() const { return m_returnVal; }

private:
	std::shared_ptr<Value> m_returnVal;
};
}