#include "Function.hpp"

#include <cassert>

#include "../Runtime/Runtime.hpp"
#include "Nothing.hpp"

namespace Kyra {
Value::Ptr Function::exec(std::vector<Value::Ptr> arguments) const {
	assert(arguments.size() == getArity());
	RuntimeContext::Ptr runtimeContext = m_definitionContext;
	for(unsigned long i = 0; i < arguments.size(); ++i) {
		const auto& name = m_functionExpr.getParameters()[i].name.getValue().asString();
		if(!runtimeContext->mutateVar(name, arguments[i]) && !runtimeContext->declareVar(name, arguments[i]))
			assert(false);
	}
	try {
		Runtime::getInstance().executeStatement(m_functionExpr.getImplementation(), runtimeContext);
	} catch(const ReturnException& exception) { return exception.getReturnVal(); }
	return Value::makePtr<Nothing>();
}
}