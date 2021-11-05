#pragma once

#include <iosfwd>
#include <string>
#include <vector>

#include "../Expressions/Expression.hpp"
#include "../HasPtrAlias.hpp"
#include "../Runtime/RuntimeContext.hpp"
#include "../Statements/ClassDeclarationStmt.hpp"
#include "../Token.hpp"
#include "Value.hpp"

namespace Slanguage {
class Klass : public Value {
	EXPR_NEEDS_VISIT_RETURN_OF_TYPE(Value::Ptr);

public:
	explicit Klass(const ClassDeclarationStmt& declarationStmt) : m_declarationStmt(declarationStmt) {}

	bool knowsIdentifier(const std::string& identifier) const;
	RuntimeContext::Ptr getInstanceContext() const { return m_instanceContext; }
	unsigned long getArity() const { return m_declarationStmt.getConstructorParameters().size(); }
	void instantiate(const std::vector<Value::Ptr>& constructorArguments);

	bool isImplicitlyTrue() const override { return true; }
	std::string getType() const override { return m_declarationStmt.getIdentifier().getValue().asString(); }
	std::string toString() const override { return "[Object of type " + getType() + "]"; }
	bool operator==(const Value::Ptr&) const override { return false; }
	bool operator<(const Value::Ptr&) const override { return false; }
	bool operator>(const Value::Ptr&) const override { return false; }

private:
	ClassDeclarationStmt m_declarationStmt;
	RuntimeContext::Ptr m_instanceContext{};
};
}