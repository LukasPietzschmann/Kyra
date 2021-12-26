#pragma once

namespace Kyra {
template <typename ValueType>
struct Variable {
	Variable(const Variable& other) = default;
	Variable(Variable&& other) noexcept = default;
	Variable(ValueType value, bool isMutable) : value(value), isMutable(isMutable) {}

	Variable& operator=(const Variable& other) = default;
	Variable& operator=(Variable&& other) noexcept = default;

	ValueType value;
	bool isMutable;
};
}