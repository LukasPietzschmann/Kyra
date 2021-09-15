#pragma once

#include <functional>
#include <unordered_map>
#include <vector>

#include "../Exceptions.hpp"
#include "../Expressions/AccessExpr.hpp"
#include "../Expressions/AssignmentExpr.hpp"
#include "../Expressions/BinaryExpr.hpp"
#include "../Expressions/CallExpr.hpp"
#include "../Expressions/Expression.hpp"
#include "../Expressions/FunctionExpr.hpp"
#include "../Expressions/GroupExpr.hpp"
#include "../Expressions/InstantiationExpr.hpp"
#include "../Expressions/LiteralExpr.hpp"
#include "../Expressions/TypeExpr.hpp"
#include "../Expressions/UnaryExpr.hpp"
#include "../Expressions/VariableExpr.hpp"
#include "../Statements/BlockStmt.hpp"
#include "../Statements/ClassDeclarationStmt.hpp"
#include "../Statements/DeclarationStmt.hpp"
#include "../Statements/ExpressionStmt.hpp"
#include "../Statements/PrintStmt.hpp"
#include "../Statements/ReturnStmt.hpp"
#include "../Statements/Statement.hpp"
#include "../Token.hpp"
#include "ClassType.hpp"
#include "FunctionType.hpp"
#include "NativeTypes.hpp"
#include "Type.hpp"

namespace LibSlg {
class TypeChecker : public ExpressionVisitor, public StatementVisitor {
private:
	struct Scope {
		Scope() : variables({}), types({}) {
			for(const auto& nativeType : Value::NativeTypes::All)
				types.emplace(nativeType, NativeTypes::make(nativeType));
		}
		std::unordered_map<std::string, Type::Ptr> variables;
		std::unordered_map<std::string, Type::Ptr> types;
	};
	EXPR_NEEDS_VISIT_RETURN_OF_TYPE(Type::Ptr);
	STMT_NEEDS_VISIT_RETURN_OF_TYPE(Scope);

public:
	static TypeChecker& getInstance();
	TypeChecker(TypeChecker const&) = delete;
	void operator=(TypeChecker const&) = delete;

	void check(const std::vector<Statement::Ptr>& statements);

	void visitAccessExpr(AccessExpr& accessExpr) override;
	void visitAssignmentExpr(AssignmentExpr& assignmentExpr) override;
	void visitBinaryExpr(BinaryExpr& binaryExpr) override;
	void visitCallExpr(CallExpr& callExpr) override;
	void visitFunction(FunctionExpr& functionExpr) override;
	void visitGroupExpr(GroupExpr& groupExpr) override;
	void visitInstantiationExpr(InstantiationExpr& instantiationExpr) override;
	void visitLiteral(LiteralExpr& literalExpr) override;
	void visitTypeExpr(TypeExpr& typeExpr) override;
	void visitUnaryExpr(UnaryExpr& unaryExpr) override;
	void visitVariable(VariableExpr& variableExpr) override;

	void visitBlockStmt(BlockStmt& blockStmt) override;
	void visitDeclarationStmt(DeclarationStmt& declarationStmt) override;
	void visitClassDeclarationStmt(ClassDeclarationStmt& classDeclarationStmt) override;
	void visitExpressionStmt(ExpressionStmt& expressionStmt) override;
	void visitPrintStmt(PrintStmt& printStmt) override;
	void visitReturnStmt(ReturnStmt& returnStmt) override;

private:
	TypeChecker() = default;

	Scope m_currentScope;
	FunctionType* m_currentFunction;

	Scope check(const Statement::Ptr& statement);
	Scope runInNewScope(const std::function<void()>& function);
};
}