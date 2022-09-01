#pragma once

#include <exception>
#include <optional>
#include <ostream>
#include <string_view>

#include "SourceRange.hpp"

class ErrorException : public std::exception {
public:
	ErrorException(std::string_view message, const SourceRange& source_range);

	const char* what() const noexcept override;
	const SourceRange& get_source_range() const;

	void print(std::ostream& stream) const;

protected:
	const std::string m_message;
	const SourceRange m_source_range;
};

template <typename T>
class ErrorOr {
public:
	ErrorOr(const T& result) : m_result(result) {}
	ErrorOr(const ErrorException& exception) : m_exception(exception) {}

	bool is_error() const { return m_exception.has_value(); }

	const T& get_result() const {
		assert(m_result.has_value());
		return *m_result;
	}

	const ErrorException& get_exception() const {
		assert(m_exception.has_value());
		return *m_exception;
	}

private:
	std::optional<ErrorException> m_exception;
	std::optional<T> m_result;
};

template <>
class ErrorOr<void> {
public:
	ErrorOr() = default;
	ErrorOr(const ErrorException& exception) : m_exception(exception) {}

	bool is_error() const { return m_exception.has_value(); }

	const ErrorException& get_exception() const {
		assert(m_exception.has_value());
		return *m_exception;
	}

private:
	std::optional<ErrorException> m_exception;
};
