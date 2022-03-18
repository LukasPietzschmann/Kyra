#pragma once

#include <iostream>
#include <ostream>
#include <string>
#include <vector>

#include "Position.hpp"

namespace Kyra {
struct EmptyValue {};

template <typename VT>
class Result {
	static_assert(!std::is_void_v<VT>, "void is not allowed as a template argument. Use EmptyValue instead.");

public:
	struct Error {
		std::string description;
		Position position;

		friend std::ostream& operator<<(std::ostream& os, const Error& error) {
			os << error.description << " [" << error.position << "]";
			return os;
		}
	};

	void reset() {
		m_errors.clear();
		delete m_value;
	};

	bool has_errors() const { return !m_errors.empty(); }
	void insert_error(Error&& error) { m_errors.emplace_back(std::move(error)); }

	template <typename... Args>
	void construct_value(Args... args) {
		static_assert(std::is_constructible_v<VT, Args...>);
		delete m_value;
		m_value = new VT(args...);
	}

	VT& assert_get_value() const {
		assert(m_value != nullptr);
		return *m_value;
	}

	const std::vector<Error>& get_errors() const { return m_errors; }

private:
	std::vector<Error> m_errors;
	VT* m_value{nullptr};
};
}