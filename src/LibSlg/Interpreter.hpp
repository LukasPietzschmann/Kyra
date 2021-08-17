#pragma once

#include <string>
#include "AstLogger.hpp"
#include "Expressions/Expression.hpp"
#include "Lexer.hpp"
#include "Parser.hpp"
#include "Statements/Statement.hpp"

namespace LibSlg {
class Interpreter : public ExpressionVisitor, public StatementVisitor {
public:
	static Interpreter& getInstance();
	Interpreter(Interpreter const&) = delete;
	void operator=(Interpreter const&) = delete;

	void execute(const std::string& code, bool verboseLogging=false);

	void visitAccessExpr(AccessExpr& accessExpr) override;
	void visitAssignmentExpr(AssignmentExpr& assignmentExpr) override;
	void visitBinaryExpr(BinaryExpr& binaryExpr) override;
	void visitCallExpr(CallExpr& callExpr) override;
	void visitFunction(Function& functionExpr) override;
	void visitLiteral(Literal& literal) override;
	void visitObject(Object& objectExpr) override;
	void visitUnaryExpr(UnaryExpr& unaryExpr) override;
	void visitVariable(Variable& variableExpr) override;
	void visitBlockStmt(BlockStmt& blockStmt) override;
	void visitDeclarationStmt(DeclarationStmt& declarationStmt) override;
	void visitExpressionStmt(ExpressionStmt& expressionStmt) override;
	void visitPrintStmt(PrintStmt& printStmt) override;
	void visitReturnStmt(ReturnStmt& returnStmt) override;
private:
	Interpreter() = default;
};
}