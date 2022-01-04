#pragma once

#include <memory>
#include <sstream>
#include <string>
#include <utility>

#include "Position.hpp"

namespace Kyra {
class MessageException : public std::exception {
public:
	MessageException() = delete;
	const char* what() const noexcept override;

protected:
	MessageException(const Position& position, std::string message);

private:
	Position m_position;
	std::string m_message;
	std::string m_computedMessage;
};

class LexerException : public MessageException {
public:
	LexerException(const Position& position, const std::string& message);
};

class ParserException : public MessageException {
public:
	ParserException(const Position& position, const std::string& message, bool unfinished = false);
	bool isUnfinished() const;

private:
	const bool m_unfinished;
};

class TypingException : public MessageException {
public:
	explicit TypingException(const std::string& message);
};

class Value;

class ReturnException : public std::exception {
public:
	explicit ReturnException(std::shared_ptr<Value> returnVal);
	const std::shared_ptr<Value>& getReturnVal() const;

private:
	std::shared_ptr<Value> m_returnVal;
};
}