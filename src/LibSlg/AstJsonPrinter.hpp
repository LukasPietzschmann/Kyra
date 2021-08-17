#pragma once

#include <sstream>
#include <string>
#include <utility>
#include "Expressions/AccessExpr.hpp"
#include "Expressions/AssignmentExpr.hpp"
#include "Expressions/BinaryExpr.hpp"
#include "Expressions/CallExpr.hpp"
#include "Expressions/Expression.hpp"
#include "Expressions/Function.hpp"
#include "Expressions/Literal.hpp"
#include "Expressions/Object.hpp"
#include "Expressions/UnaryExpr.hpp"
#include "Expressions/Variable.hpp"
#include "Statements/BlockStmt.hpp"
#include "Statements/DeclarationStmt.hpp"
#include "Statements/ExpressionStmt.hpp"
#include "Statements/PrintStmt.hpp"
#include "Statements/ReturnStmt.hpp"
#include "Statements/Statement.hpp"
#include "TokenType.hpp"

namespace LibSlg {
class AstJsonPrinter : public ExpressionVisitor, public StatementVisitor {
public:
	explicit AstJsonPrinter(Statement::Ptr statement) : m_statements({std::move(statement)}) {}
	explicit AstJsonPrinter(std::vector<Statement::Ptr> statements) : m_statements(std::move(statements)) {}

	std::string dump();

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
	std::vector<Statement::Ptr> m_statements;
	std::stringstream m_output;
};
}