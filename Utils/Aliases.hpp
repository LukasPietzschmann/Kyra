#pragma once

#include <cassert>
#include <memory>
#include <type_traits>

#define assert_not_reached() assert(0 && "should not reach")

template <typename T>
using RefPtr = std::shared_ptr<T>;

template <typename T, typename... Args>
RefPtr<T> inline mk_ref(Args... args) {
	return std::make_shared<T>(args...);
}

template <typename T, typename... Args>
using All = typename std::enable_if_t<std::conjunction_v<std::is_convertible<Args, T>...>>;
