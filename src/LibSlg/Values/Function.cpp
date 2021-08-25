#include "Function.hpp"
#include "../Interpreter.hpp"

namespace LibSlg {
Value::Ptr Function::exec(std::vector<Value::Ptr> arguments) {
	assert(arguments.size() == getArity());
	Context::Ptr runtimeContext = Context::makePtr(m_definitionContext);
	for(unsigned long i = 0; i < arguments.size(); ++i)
		runtimeContext->declare(m_functionExpr.getParameters()[i].getValue().asString(), arguments[i]);
	auto block = std::dynamic_pointer_cast<BlockStmt>(m_functionExpr.getImplementation());
	try {
		Interpreter::getInstance().executeStatementsOnContext(block->getStatements(), runtimeContext);
	}catch(ReturnException& exception) {
		return exception.getReturnVal();
	}
	return Value::makePtr<Nothing>();
}
}