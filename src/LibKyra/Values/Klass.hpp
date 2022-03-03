#pragma once

#include <string>
#include <vector>

#include "../Expressions/Expression.hpp"
#include "../HasPtrAlias.hpp"
#include "../Runtime/RuntimeContext.hpp"
#include "../Statements/ClassDeclarationStmt.hpp"
#include "../Token.hpp"
#include "Value.hpp"

namespace Kyra {
class Klass : public Value {
	EXPR_NEEDS_VISIT_RETURN_OF_TYPE(Value::Ptr);

public:
	explicit Klass(const ClassDeclarationStmt& declaration_stmt) : m_declaration_stmt(declaration_stmt) {}

	RuntimeContext::Ptr get_instance_context() const { return m_instance_context; }
	void instantiate(const std::vector<Value::Ptr>& constructor_arguments);
	Value::Ptr get_declaration(const std::string& name) const;

	bool is_native() const override { return false; }
	bool is_implicitly_true() const override { return true; }
	std::string get_type() const override { return m_declaration_stmt.get_identifier().get_value().as_string(); }
	std::string to_string() const override { return "[Object of type " + get_type() + "]"; }
	bool operator==(const Value::Ptr&) const override { return false; }
	bool operator<(const Value::Ptr&) const override { return false; }
	bool operator>(const Value::Ptr&) const override { return false; }

private:
	ClassDeclarationStmt m_declaration_stmt;
	RuntimeContext::Ptr m_instance_context{};
};
}