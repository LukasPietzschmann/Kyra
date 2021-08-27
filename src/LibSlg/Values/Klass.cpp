#include "Klass.hpp"
#include "../Context.hpp"
#include "../Interpreter.hpp"

namespace LibSlg {
void Klass::instantiate(std::vector<Value::Ptr> constructorArguments) {
	assert(constructorArguments.size() == getArity());
	m_instanceContext = Context::makePtr();
	for(unsigned long i = 0; i < constructorArguments.size(); ++i) {
		assert(m_declarationStmt.getConstructorParameters()[i].type == constructorArguments[i]->getType());
		m_instanceContext->declareVar(m_declarationStmt.getConstructorParameters()[i].name.getValue().asString(),
				constructorArguments[i], constructorArguments[i]->getType(),
				m_declarationStmt.getConstructorParameters()[i].isMutable);
	}
	for(const auto& decl: m_declarationStmt.getDeclarations())
		m_instanceContext->declareVar(decl->getIdentifier().getValue().asString(),
				decl->getInitializer()->accept(Interpreter::getInstance()), decl->getType(), decl->isMutable());
}
}