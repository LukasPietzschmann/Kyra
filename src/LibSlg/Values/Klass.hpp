#pragma once

#include "Value.hpp"
#include "../Statements/ClassDeclarationStmt.hpp"

namespace LibSlg {
class Context;

class Klass : public Value {
NEEDS_VISIT_RETURN_OF_TYPE(Value::Ptr);
public:
	explicit Klass(const ClassDeclarationStmt& declarationStmt) : m_declarationStmt(declarationStmt) {}

	bool knowsIdentifier(const std::string& identifier) const;
	const std::shared_ptr<Context>& getInstanceContext() const { return m_instanceContext; }
	unsigned int getArity() const { return m_declarationStmt.getConstructorParameters().size(); }
	void instantiate(std::vector<Value::Ptr> constructorArguments);

	bool isImplicitlyTrue() const override { return true; }
	Type getType() const override { return m_declarationStmt.getIdentifier().getValue().asString(); }
	std::string toString() const override { return "[Object of type " + getType() + "]"; }
	bool operator==(const Value::Ptr&) const override { return false; }
	bool operator<(const Value::Ptr&) const override { return false; }
	bool operator>(const Value::Ptr&) const override { return false; }
private:
	ClassDeclarationStmt m_declarationStmt;
	std::shared_ptr<Context> m_instanceContext;
};
}