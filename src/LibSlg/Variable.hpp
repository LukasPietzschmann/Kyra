#pragma once

namespace Slanguage {
template <typename ValueType>
struct Variable {
	Variable(ValueType value, bool isMutable) : value(value), isMutable(isMutable) {}
	ValueType value;
	bool isMutable;
};
}