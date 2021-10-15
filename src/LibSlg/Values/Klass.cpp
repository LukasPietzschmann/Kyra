#include "Klass.hpp"

#include "../Interpreter.hpp"
#include "../Runtime/RuntimeContext.hpp"

namespace Slanguage {
bool Klass::knowsIdentifier(const std::string& identifier) const {
	bool declarationsContainIdentifier = std::any_of(m_declarationStmt.getDeclarations().begin(),
			m_declarationStmt.getDeclarations().end(),
			[&identifier](const std::shared_ptr<DeclarationStmt>& decl) {
				return decl->getIdentifier().getValue().asString() == identifier;
			});
	bool constructorContainsIdentifier = std::any_of(m_declarationStmt.getConstructorParameters().begin(),
			m_declarationStmt.getConstructorParameters().end(),
			[&identifier](const ClassDeclarationStmt::ConstructorParameter& param) {
				return param.name.getValue().asString() == identifier;
			});
	return declarationsContainIdentifier || constructorContainsIdentifier;
}

void Klass::instantiate(const std::vector<Value::Ptr>& constructorArguments) {
	assert(constructorArguments.size() == getArity());
	m_instanceContext = new RuntimeContext();
	for(unsigned long i = 0; i < constructorArguments.size(); ++i) {
		assert(m_declarationStmt.getConstructorParameters()[i].type == constructorArguments[i]->getType());
		m_instanceContext->declareVar(m_declarationStmt.getConstructorParameters()[i].name.getValue().asString(),
				constructorArguments[i],
				m_declarationStmt.getConstructorParameters()[i].isMutable);
	}
	for(const auto& decl : m_declarationStmt.getDeclarations()) {
		Value::Ptr init = Value::makePtr<Nothing>();
		if(decl->getInitializer() != nullptr)
			init = Runtime::getInstance().executeExpression(decl->getInitializer(), m_instanceContext);
		m_instanceContext->declareVar(decl->getIdentifier().getValue().asString(), init, decl->isMutable());
	}
}
}