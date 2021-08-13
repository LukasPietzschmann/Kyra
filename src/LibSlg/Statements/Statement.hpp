#pragma once

#include "Forward.hpp"

namespace LibSlg {

class StatementVisitor {
public:
	virtual void visitBlockStmt(BlockStmt& blockStmt) = 0;
	virtual void visitDeclarationStmt(DeclarationStmt& declarationStmt) = 0;
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
};
}