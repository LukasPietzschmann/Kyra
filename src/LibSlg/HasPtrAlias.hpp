#pragma once

#include <cassert>
#include <memory>
#include <string>

namespace Slanguage {
template <typename C>
class HasPtrAlias {
public:
	HasPtrAlias() = default;
	~HasPtrAlias() = default;

	using Ptr = std::shared_ptr<C>;
	using WeakPtr = std::weak_ptr<C>;

	template <typename T, typename... Args>
	static inline Ptr makePtr(Args... args) {
		return std::make_shared<T>(args...);
	}

	static inline Ptr lock(WeakPtr weakPtr) {
		auto sp = weakPtr.lock();
		if(sp == nullptr)
			assert(false);
		return sp;
	}
};
}