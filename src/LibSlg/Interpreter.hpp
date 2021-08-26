#pragma once

#include <string>
#include "AstLogger.hpp"
#include "Context.hpp"
#include "Expressions/Expression.hpp"
#include "Values/Function.hpp"
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
	void executeStatementsOnContext(const std::vector<Statement::Ptr>& statements, const Context::Ptr& context);
	bool isIncompleteStatement(const std::string& code);

	Value::Ptr visitAccessExpr(AccessExpr& accessExpr) override;
	Value::Ptr visitAssignmentExpr(AssignmentExpr& assignmentExpr) override;
	Value::Ptr visitBinaryExpr(BinaryExpr& binaryExpr) override;
	Value::Ptr visitCallExpr(CallExpr& callExpr) override;
	Value::Ptr visitFunction(FunctionExpr& functionExpr) override;
	Value::Ptr visitGroupExpr(GroupExpr& groupExpr) override;
	Value::Ptr visitLiteral(LiteralExpr& literalExpr) override;
	Value::Ptr visitUnaryExpr(UnaryExpr& unaryExpr) override;
	Value::Ptr visitVariable(VariableExpr& variableExpr) override;
	void visitBlockStmt(BlockStmt& blockStmt) override;
	void visitDeclarationStmt(DeclarationStmt& declarationStmt) override;
	void visitExpressionStmt(ExpressionStmt& expressionStmt) override;
	void visitPrintStmt(PrintStmt& printStmt) override;
	void visitReturnStmt(ReturnStmt& returnStmt) override;
private:
	Interpreter() : m_currentContext(Context::makePtr()) {};

	Context::Ptr m_currentContext;

};
}