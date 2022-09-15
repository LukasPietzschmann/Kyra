#pragma once

#include <cassert>
#include <memory>
#include <type_traits>
#include <utility>

#define assert_not_reached() assert(0 && "should not reach")

template <typename T>
using RefPtr = std::shared_ptr<T>;

template <typename T>
using OwnPtr = std::unique_ptr<T>;

template <typename T, typename... Args>
RefPtr<T> inline mk_ref(Args&&... args) {
	return std::make_shared<T>(std::forward<Args>(args)...);
}

template <typename T, typename... Args>
OwnPtr<T> inline mk_own(Args&&... args) {
	return std::make_unique<T>(std::forward<Args>(args)...);
}

template <typename T, typename... Args>
using All = typename std::enable_if_t<std::conjunction_v<std::is_convertible<Args, T>...>>;

#define VISIT_RETURN_TYPE(type)                                   \
private:                                                          \
	using VisitType = type;                                       \
	VisitType _return_value;                                      \
	bool _has_return_value{false};                                \
	template <typename... Args>                                   \
	auto _construct_return_type(Args... args) {                   \
		if constexpr(std::is_pointer_v<VisitType>)                \
			return new std::remove_pointer_t<VisitType>(args...); \
		else                                                      \
			return std::remove_pointer_t<VisitType>(args...);     \
	}

#define return_from_visit_emplace(...)                       \
	do {                                                     \
		_return_value = _construct_return_type(__VA_ARGS__); \
		_has_return_value = true;                            \
		return;                                              \
	} while(0)

#define return_from_visit(value)  \
	do {                          \
		_return_value = value;    \
		_has_return_value = true; \
		return;                   \
	} while(0)

// https://gcc.gnu.org/onlinedocs/gcc/Statement-Exprs.html#Statement-Exprs
#define visit_with_return(visitee)                                                \
	({                                                                            \
		_has_return_value = false;                                                \
		(visitee).accept(*this);                                                  \
		assert(_has_return_value && "no value was returned from previous visit"); \
		_return_value;                                                            \
	})
