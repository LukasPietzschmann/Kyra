#include "Function.hpp"

#include <cassert>
#include <memory>

#include "../Exceptions.hpp"
#include "../Expressions/FunctionExpr.hpp"
#include "../HasPtrAlias.hpp"
#include "../Runtime/Runtime.hpp"
#include "../Runtime/RuntimeContext.hpp"
#include "../Token.hpp"
#include "Nothing.hpp"

namespace Slanguage {
Value::Ptr Function::exec(std::vector<Value::Ptr> arguments) const {
	assert(arguments.size() == getArity());
	RuntimeContext::Ptr runtimeContext = m_definitionContext;
	for(unsigned long i = 0; i < arguments.size(); ++i)
		runtimeContext->declareVar(m_functionExpr.getParameters()[i].name.getValue().asString(), arguments[i]);
	try {
		Runtime::getInstance().executeStatement(m_functionExpr.getImplementation(), runtimeContext);
	} catch(const ReturnException& exception) { return exception.getReturnVal(); }
	return Value::makePtr<Nothing>();
}
}