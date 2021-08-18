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
#include "Expressions/GroupExpr.hpp"
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
	std::vector<Statement::Ptr> m_statements;
	std::stringstream m_output;
};
}