#pragma once

namespace Kyra {
template <typename ValueType>
struct Variable {
	Variable(const Variable& other) = default;
	Variable(Variable&& other) noexcept = default;
	Variable(ValueType value, bool is_mutable) : value(value), is_mutable(is_mutable) {}

	Variable& operator=(const Variable& other) = default;
	Variable& operator=(Variable&& other) noexcept = default;

	ValueType value;
	bool is_mutable;
};
}