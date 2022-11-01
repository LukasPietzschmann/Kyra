#pragma once

#include <cassert>
#include <memory>
#include <stdexcept>
#include <type_traits>
#include <utility>
#include <vector>

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

template <typename T1, typename T2>
std::vector<std::pair<T1, T2>> zip(const std::vector<T1>& v1, const std::vector<T2>& v2, bool strict = false) {
	unsigned v1_size = v1.size();
	unsigned v2_size = v2.size();
	if(strict && v1_size != v2_size)
		throw std::invalid_argument("If the strict flag is set, both vectors have to be of the same size!");

	std::vector<std::pair<T1, T2>> result;
	for(unsigned i = 0; i < std::min(v1_size, v2_size); ++i)
		result.emplace_back(v1[i], v2[i]);

	return result;
}

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
