#pragma once

#include <exception>
#include <ostream>
#include <string_view>

#include "SourceRange.hpp"

class ErrorException : public std::exception {
public:
	friend class Error;
	ErrorException(std::string_view message, const SourceRange& source_range);

	const char* what() const noexcept override;

protected:
	const std::string_view m_message;
	const SourceRange& m_source_range;
};

class Error {
public:
	explicit Error(const ErrorException& exception);

	void print(std::ostream& stream) const;

private:
	const ErrorException& m_exception;
};
