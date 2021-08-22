#pragma once

#include <stack>
#include "Context.hpp"
#include "Expressions/FunctionExpr.hpp"

namespace LibSlg {
class Function {
public:
	typedef std::shared_ptr<Function> Ptr;
	Function(const FunctionExpr& functionExpr, const Context::Ptr& definitionContext) :
			m_functionExpr(functionExpr), m_definitionContext(definitionContext) {}
	const FunctionExpr& getFunctionExpr() const { return m_functionExpr; }
	const Context::Ptr& getContext() const { return m_definitionContext; }
	unsigned int getArity() const { return m_functionExpr.getParameters().size(); }

	Value::Ptr exec(std::vector<Value::Ptr> arguments);

	template <class... Args>
	static Function::Ptr makePtr(Args... args) {
		static_assert(std::is_constructible<Function, Args...>::value, "Cannot construct object in Function::makePtr");
		return std::make_shared<Function>(args...);
	}
private:
	FunctionExpr m_functionExpr;
	Context::Ptr m_definitionContext;
};
}