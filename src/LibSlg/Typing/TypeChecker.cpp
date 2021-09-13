#include "TypeChecker.hpp"

namespace LibSlg {
TypeChecker& TypeChecker::getInstance() {
	static TypeChecker instance;
	return instance;
}

void TypeChecker::check(const std::vector<Statement::Ptr>& statements) {
	for(const auto& statement : statements)
		check(statement);
}

TypeChecker::Scope TypeChecker::check(const Statement::Ptr& statement) {
	STMT_ACCEPT(statement, *this, Scope result);
	return result;
}

TypeChecker::Scope TypeChecker::runInNewScope(const std::function<void()>& function) {
}

void TypeChecker::visitAccessExpr(AccessExpr& accessExpr) {
}

void TypeChecker::visitAssignmentExpr(AssignmentExpr& assignmentExpr) {
}

void TypeChecker::visitBinaryExpr(BinaryExpr& binaryExpr) {}

void TypeChecker::visitCallExpr(CallExpr& callExpr) {}

void TypeChecker::visitFunction(FunctionExpr& functionExpr) {
}

void TypeChecker::visitGroupExpr(GroupExpr& groupExpr) {
}

void TypeChecker::visitInstantiationExpr(InstantiationExpr& instantiationExpr) {
}

void TypeChecker::visitLiteral(LiteralExpr& literalExpr) {
	EXPR_RETURN_FROM_VISIT(NativeTypes::make(literalExpr.getValue()->getType()));
}

void TypeChecker::visitUnaryExpr(UnaryExpr& unaryExpr) {
	}
}

void TypeChecker::visitVariable(VariableExpr& variableExpr) {
}

void TypeChecker::visitBlockStmt(BlockStmt& blockStmt) {
}

void TypeChecker::visitDeclarationStmt(DeclarationStmt& declarationStmt) {
}

void TypeChecker::visitClassDeclarationStmt(ClassDeclarationStmt& classDeclarationStmt) {
}

void TypeChecker::visitExpressionStmt(ExpressionStmt& expressionStmt) {
}

void TypeChecker::visitPrintStmt(PrintStmt& printStmt) {
}

void TypeChecker::visitReturnStmt(ReturnStmt& returnStmt) {
}
}