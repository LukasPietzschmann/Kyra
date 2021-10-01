#include "Function.hpp"

#include "../Interpreter.hpp"

namespace LibSlg {
Value::Ptr Function::exec(std::vector<Value::Ptr> arguments) const {
	assert(arguments.size() == getArity());
	Context::Ptr runtimeContext = Context::makePtr(m_definitionContext);
	for(unsigned long i = 0; i < arguments.size(); ++i)
		runtimeContext->declareVar(m_functionExpr.getParameters()[i].name.getValue().asString(), arguments[i]);
	auto block = std::dynamic_pointer_cast<BlockStmt>(m_functionExpr.getImplementation());
	try {
		Interpreter::getInstance().executeStatementsOnContext(block->getStatements(), runtimeContext);
	} catch(const ReturnException& exception) { return exception.getReturnVal(); }
	return Value::makePtr<Nothing>();
}
}