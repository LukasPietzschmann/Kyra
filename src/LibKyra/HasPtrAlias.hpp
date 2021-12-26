#pragma once

#include <cassert>
#include <memory>
#include <string>

namespace Kyra {
template <typename C>
class HasPtrAlias {
public:
	HasPtrAlias() = default;
	~HasPtrAlias() = default;

	using Ptr = std::shared_ptr<C>;

	template <typename T, typename... Args>
	static inline Ptr makePtr(Args... args) {
		return std::make_shared<T>(args...);
	}
};
}