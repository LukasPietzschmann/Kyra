#pragma once

#include <memory>
#include <string>

namespace Slanguage {
template <typename C>
class HasPtrAlias {
public:
	HasPtrAlias() = default;
	~HasPtrAlias() = default;

	using Ptr = std::shared_ptr<C>;
	template <typename T, typename... Args>
	static Ptr makePtr(Args... args) {
		return std::make_shared<T>(args...);
	}
};
}