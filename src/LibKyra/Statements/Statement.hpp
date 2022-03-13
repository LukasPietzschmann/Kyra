#pragma once

#include "../HasPtrAlias.hpp"
#include "../Position.hpp"
#include "Forward.hpp"

namespace Kyra {

class StatementVisitor {
public:
#define STMT_NEEDS_VISIT_RETURN_OF_TYPE(type) \
private:                                      \
	type m_stmtVisitorResult
#define STMT_RETURN_FROM_VISIT(value) \
	do {                              \
		m_stmtVisitorResult = value;  \
		return;                       \
	} while(0)
#define STMT_GET_FROM_VISIT(dest) dest = m_stmtVisitorResult
#define STMT_ACCEPT(visitee, visitor, dest) \
	visitee->accept(visitor);               \
	STMT_GET_FROM_VISIT(dest)

	virtual ~StatementVisitor() = default;

	virtual void visit_block_stmt(BlockStmt& block_stmt) = 0;
	virtual void visit_var_declaration_stmt(VarDeclarationStmt& declaration_stmt) = 0;
	virtual void visit_class_declaration_stmt(ClassDeclarationStmt& class_declaration_stmt) = 0;
	virtual void visit_fun_declaration_stmt(FunDeclarationStmt& fun_declaration_stmt) = 0;
	virtual void visit_expression_stmt(ExpressionStmt& expression_stmt) = 0;
	virtual void visit_print_stmt(PrintStmt& print_stmt) = 0;
	virtual void visit_return_stmt(ReturnStmt& return_stmt) = 0;
	virtual void visit_while_stmt(WhileStmt& while_stmt) = 0;
	virtual void visit_if_stmt(IfStmt& if_stmt) = 0;
};

class Statement : public HasPtrAlias<Statement> {
public:
	explicit Statement(const Position& position) : m_position(position) {}
	virtual ~Statement() = default;

	virtual void accept(StatementVisitor& visitor) = 0;

	const Position& get_position() const { return m_position; }

protected:
	Position m_position;
};
}