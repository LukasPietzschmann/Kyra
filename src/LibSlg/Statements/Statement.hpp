#pragma once

#include "../Position.hpp"
#include "Forward.hpp"

namespace LibSlg {

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

	virtual void visitBlockStmt(BlockStmt& blockStmt) = 0;
	virtual void visitDeclarationStmt(DeclarationStmt& declarationStmt) = 0;
	virtual void visitClassDeclarationStmt(ClassDeclarationStmt& classDeclarationStmt) = 0;
	virtual void visitExpressionStmt(ExpressionStmt& expressionStmt) = 0;
	virtual void visitPrintStmt(PrintStmt& printStmt) = 0;
	virtual void visitReturnStmt(ReturnStmt& returnStmt) = 0;
};

class Statement {
public:
	using Ptr = std::shared_ptr<Statement>;
	explicit Statement(const Position& position) : m_position(position) {}
	virtual ~Statement() = default;

	virtual void accept(StatementVisitor& visitor) = 0;
	template <typename T, class... Args>
	static Ptr makePtr(Args... args) {
		static_assert(std::is_constructible_v<T, Args...>, "Cannot construct object in Statement::makePtr");
		return std::make_shared<T>(args...);
	}

	const Position& getPosition() const { return m_position; }

protected:
	Position m_position;
};
}