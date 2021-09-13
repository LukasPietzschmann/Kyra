#pragma once

#include <string>

#include "AstLogger.hpp"
#include "Context.hpp"
#include "Expressions/Expression.hpp"
#include "Lexer.hpp"
#include "Parser.hpp"
#include "Statements/Statement.hpp"
#include "Typing/TypeChecker.hpp"
#include "Values/Function.hpp"
#include "Values/Klass.hpp"

namespace LibSlg {
class Interpreter : public ExpressionVisitor, public StatementVisitor {
	EXPR_NEEDS_VISIT_RETURN_OF_TYPE(Value::Ptr);

public:
	static Interpreter& getInstance();
	Interpreter(Interpreter const&) = delete;
	void operator=(Interpreter const&) = delete;

	// FIXME this is only needed for Klass::instantiate
	Value::Ptr getVisitorReturn() const { return m_exprVisitorResult; }

	void execute(const std::string& code, bool verboseLogging = false, bool passThroughExceptions = false);
	void executeStatementsOnContext(const std::vector<Statement::Ptr>& statements, const Context::Ptr& context);
	bool isIncompleteStatement(const std::string& code);

	void visitAccessExpr(AccessExpr& accessExpr) override;
	void visitAssignmentExpr(AssignmentExpr& assignmentExpr) override;
	void visitBinaryExpr(BinaryExpr& binaryExpr) override;
	void visitCallExpr(CallExpr& callExpr) override;
	void visitFunction(FunctionExpr& functionExpr) override;
	void visitGroupExpr(GroupExpr& groupExpr) override;
	void visitInstantiationExpr(InstantiationExpr& instantiationExpr) override;
	void visitLiteral(LiteralExpr& literalExpr) override;
	void visitUnaryExpr(UnaryExpr& unaryExpr) override;
	void visitVariable(VariableExpr& variableExpr) override;
	void visitBlockStmt(BlockStmt& blockStmt) override;
	void visitDeclarationStmt(DeclarationStmt& declarationStmt) override;
	void visitClassDeclarationStmt(ClassDeclarationStmt& classDeclarationStmt) override;
	void visitExpressionStmt(ExpressionStmt& expressionStmt) override;
	void visitPrintStmt(PrintStmt& printStmt) override;
	void visitReturnStmt(ReturnStmt& returnStmt) override;

private:
	Interpreter() : m_currentContext(Context::makePtr()){};

	Context::Ptr m_currentContext;
};
}