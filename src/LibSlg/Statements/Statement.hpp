#pragma once

#include "Forward.hpp"

namespace LibSlg {
class Statement {
public:
	typedef std::shared_ptr<Statement> Ptr;
	template <typename T, class... Args>
	static Ptr makePtr(Args... args) {
		static_assert(std::is_constructible<T, Args...>::value, "Cannot construct object in Statement::makePtr");
		return std::make_shared<T>(args...);
	}
};
}