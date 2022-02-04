#include "Klass.hpp"

#include <cassert>
#include <memory>

#include "../Runtime/Runtime.hpp"
#include "../Statements/DeclarationStmt.hpp"
#include "Nothing.hpp"

namespace Kyra {
void Klass::instantiate(const std::vector<Value::Ptr>& constructorArguments) {
	assert(constructorArguments.size() == m_declarationStmt.getConstructorParameters().size());
	m_instanceContext = RuntimeContext::makePtr<RuntimeContext>();
	for(unsigned long i = 0; i < constructorArguments.size(); ++i) {
		assert(m_declarationStmt.getConstructorParameters()[i].type == constructorArguments[i]->getType());
		if(!m_instanceContext->declareVar(m_declarationStmt.getConstructorParameters()[i].name.getValue().asString(),
				   constructorArguments[i],
				   m_declarationStmt.getConstructorParameters()[i].isMutable))
			assert(false);
	}
	for(const auto& decl : m_declarationStmt.getDeclarations()) {
		Value::Ptr init = Value::makePtr<Nothing>();
		if(decl->getInitializer() != nullptr)
			init = Runtime::getInstance().executeExpression(decl->getInitializer(), m_instanceContext);
		if(!m_instanceContext->declareVar(decl->getIdentifier().getValue().asString(), init, decl->isMutable()))
			assert(false);
	}
}

Value::Ptr Klass::getDeclaration(const std::string& name) const {
	const auto res = m_instanceContext->getVar(name);
	assert(res.has_value());
	return res->value;
}
}