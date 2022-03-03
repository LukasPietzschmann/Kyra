#include "Exceptions.hpp"

#include <sstream>

namespace Kyra {
MessageException::MessageException(const Position& position, std::string message) :
	m_position(position), m_message(std::move(message)) {
	std::stringstream ss;
	ss << m_message << " [" << m_position.start << " - " << m_position.end << "]";
	m_computed_message = ss.str();
}

const char* MessageException::what() const noexcept { return m_computed_message.c_str(); }

LexerException::LexerException(const Position& position, const std::string& message) :
	MessageException(position, message) {}

ParserException::ParserException(const Position& position, const std::string& message, bool unfinished) :
	MessageException(position, message), m_unfinished(unfinished) {}

bool ParserException::is_unfinished() const { return m_unfinished; }

TypingException::TypingException(const std::string& message) : MessageException(Position(0, 0, 0, 0), message) {}

ReturnException::ReturnException(std::shared_ptr<Value> return_val) : m_return_val(std::move(return_val)) {}

const std::shared_ptr<Value>& ReturnException::get_return_val() const { return m_return_val; }
}