#include "Function.hpp"

#include "../Interpreter.hpp"

namespace Slanguage {
Value::Ptr Function::exec(std::vector<Value::Ptr> arguments) const {
	assert(arguments.size() == getArity());
	RuntimeContext::Ptr runtimeContext = m_definitionContext;
	for(unsigned long i = 0; i < arguments.size(); ++i)
		runtimeContext->declareVar(m_functionExpr.getParameters()[i].name.getValue().asString(), arguments[i]);
	auto block = std::dynamic_pointer_cast<BlockStmt>(m_functionExpr.getImplementation());
	try {
		for(const auto& statement : block->getStatements())
			Runtime::getInstance().executeStatement(statement, runtimeContext);
	} catch(const ReturnException& exception) { return exception.getReturnVal(); }
	return Value::makePtr<Nothing>();
}
}