#include "Klass.hpp"

#include "../Context.hpp"
#include "../Interpreter.hpp"

namespace LibSlg {
bool Klass::knowsIdentifier(const std::string& identifier) const {
	bool declarationsContainIdentifier =
			std::any_of(m_declarationStmt.getDeclarations().begin(), m_declarationStmt.getDeclarations().end(),
					[&identifier](const std::shared_ptr<DeclarationStmt>& decl) -> bool {
						return decl->getIdentifier().getValue().asString() == identifier;
					});
	bool constructorContainsIdentifier = std::any_of(m_declarationStmt.getConstructorParameters().begin(),
			m_declarationStmt.getConstructorParameters().end(),
			[&identifier](const ClassDeclarationStmt::ConstructorParameter& param) -> bool {
				return param.name.getValue().asString() == identifier;
			});
	return declarationsContainIdentifier || constructorContainsIdentifier;
}

void Klass::instantiate(std::vector<Value::Ptr> constructorArguments) {
	assert(constructorArguments.size() == getArity());
	m_instanceContext = Context::makePtr();
	for(unsigned long i = 0; i < constructorArguments.size(); ++i) {
		assert(m_declarationStmt.getConstructorParameters()[i].type == constructorArguments[i]->getType());
		m_instanceContext->declareVar(m_declarationStmt.getConstructorParameters()[i].name.getValue().asString(),
				constructorArguments[i], constructorArguments[i]->getType(),
				m_declarationStmt.getConstructorParameters()[i].isMutable);
	}
	for(const auto& decl : m_declarationStmt.getDeclarations()) {
		Value::Ptr init = Value::makePtr<Nothing>();
		if(decl->getInitializer() != nullptr) {
			EXPR_ACCEPT(decl->getInitializer(), Interpreter::getInstance(), init);
		}
		m_instanceContext->declareVar(
				decl->getIdentifier().getValue().asString(), init, decl->getType(), decl->isMutable());
	}
}
}