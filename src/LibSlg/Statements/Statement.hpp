#pragma once

#include "Forward.hpp"

namespace LibSlg {

class StatementVisitor {
public:
#define STMT_NEEDS_VISIT_RETURN_OF_TYPE(type) private: type m_stmtVisitorResult
#define STMT_RETURN_FROM_VISIT(value) m_stmtVisitorResult = value; return
#define STMT_GET_FROM_VISIT(dest) dest = m_stmtVisitorResult
#define STMT_ACCEPT(visitee, visitor, dest) visitee->accept(visitor); STMT_GET_FROM_VISIT(dest)

	virtual void visitBlockStmt(BlockStmt& blockStmt) = 0;
	virtual void visitDeclarationStmt(DeclarationStmt& declarationStmt) = 0;
	virtual void visitClassDeclarationStmt(ClassDeclarationStmt& classDeclarationStmt) = 0;
	virtual void visitExpressionStmt(ExpressionStmt& expressionStmt) = 0;
	virtual void visitPrintStmt(PrintStmt& printStmt) = 0;
	virtual void visitReturnStmt(ReturnStmt& returnStmt) = 0;
};

class Statement {
public:
	typedef std::shared_ptr<Statement> Ptr;
	virtual void accept(StatementVisitor& visitor) = 0;
	template <typename T, class... Args>
	static Ptr makePtr(Args... args) {
		static_assert(std::is_constructible<T, Args...>::value, "Cannot construct object in Statement::makePtr");
		return std::make_shared<T>(args...);
	}
protected:
	virtual ~Statement() {}
};
}