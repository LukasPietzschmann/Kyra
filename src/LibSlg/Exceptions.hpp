#pragma once

#include <string>
#include <utility>

namespace LibSlg {
class ParserException : std::exception {
public:
	explicit ParserException(std::string message) : m_message(std::move(message)) {}
	const char* what() const noexcept override { return m_message.c_str(); }
private:
	std::string m_message;
};

class RuntimeException : std::exception {
public:
	explicit RuntimeException(std::string message) : m_message(std::move(message)) {}
	const char* what() const noexcept override { return m_message.c_str(); }
private:
	std::string m_message;
};
}