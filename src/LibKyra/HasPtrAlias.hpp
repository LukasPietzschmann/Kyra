#pragma once

#include <memory>

namespace Kyra {
template <typename C>
class HasPtrAlias {
public:
	HasPtrAlias() = default;
	~HasPtrAlias() = default;

	using Ptr = std::shared_ptr<C>;

	template <typename T, typename... Args>
	static inline Ptr make_ptr(Args... args) {
		return std::make_shared<T>(args...);
	}
};
}