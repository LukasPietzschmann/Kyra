#pragma once

#include <string>
#include "AstLogger.hpp"
#include "Context.hpp"
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

	void execute(const std::string& code, bool verboseLogging=false, bool passThroughExceptions=false);

	Value::Ptr visitAccessExpr(AccessExpr& accessExpr) override;
	Value::Ptr visitAssignmentExpr(AssignmentExpr& assignmentExpr) override;
	Value::Ptr visitBinaryExpr(BinaryExpr& binaryExpr) override;
	Value::Ptr visitCallExpr(CallExpr& callExpr) override;
	Value::Ptr visitFunction(Function& functionExpr) override;
	Value::Ptr visitGroupExpr(GroupExpr& groupExpr) override;
	Value::Ptr visitLiteral(Literal& literal) override;
	Value::Ptr visitObject(Object& objectExpr) override;
	Value::Ptr visitUnaryExpr(UnaryExpr& unaryExpr) override;
	Value::Ptr visitVariable(Variable& variableExpr) override;
	void visitBlockStmt(BlockStmt& blockStmt) override;
	void visitDeclarationStmt(DeclarationStmt& declarationStmt) override;
	void visitExpressionStmt(ExpressionStmt& expressionStmt) override;
	void visitPrintStmt(PrintStmt& printStmt) override;
	void visitReturnStmt(ReturnStmt& returnStmt) override;
private:
	Interpreter() = default;

	Context m_global;
};
}