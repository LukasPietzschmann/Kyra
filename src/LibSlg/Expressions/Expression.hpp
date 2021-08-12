#pragma once

namespace LibSlg {
class Expression {
public:
	typedef std::shared_ptr<Expression> Ptr;
	template <typename T, class... Args>
	static Ptr makePtr(Args... args) {
		static_assert(std::is_constructible<T, Args...>::value, "Cannot construct object in Expression::makePtr");
		return std::make_shared<T>(args...);
	}
};
}