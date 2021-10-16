#pragma once

#include <functional>
#include <memory>
#include <optional>
#include <tuple>
#include <unordered_map>
#include <utility>
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
#include "TypeContext.hpp"
#include "TypingErrors.hpp"

namespace Slanguage {
class TypeChecker : public ExpressionVisitor, public StatementVisitor {
#define THROW_TYPING_ERROR(error)               \
	do {                                        \
		m_errors.push_back((error).getCause()); \
		return;                                 \
	} while(0)

	EXPR_NEEDS_VISIT_RETURN_OF_TYPE(Type::Ptr);

public:
	class Result {
	public:
		bool hasErrors() const { return m_errors.has_value(); }
		void insertError(std::string message) {
			if(!m_errors.has_value())
				m_errors = std::make_optional<std::vector<std::string>>({std::move(message)});
			else
				m_errors->push_back(std::move(message));
		}
		std::vector<std::string> getErrors() const { return m_errors.value_or(std::vector<std::string>()); }

	private:
		std::optional<std::vector<std::string>> m_errors{std::nullopt};
	};

	static TypeChecker& getInstance();
	TypeChecker(TypeChecker const&) = delete;
	TypeChecker(TypeChecker&&) = delete;
	void operator=(TypeChecker const&) = delete;
	void operator=(TypeChecker&&) = delete;

	TypeChecker::Result check(const std::vector<Statement::Ptr>& statements);

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

	std::vector<std::string> m_errors;
	TypeContext::Ptr m_currentScope{TypeContext::makePtr<TypeContext>()};
	FunctionType* m_currentFunction{};
	bool m_doesCurrentFunctionReturn{};
	char* m_currentClassName{};

	void check(const Statement::Ptr& statement);
	template <class Callback>
	TypeContext::Ptr runInNewScope(const Callback& function,
			TypeContext::Ptr parent,
			TypeContext::Ptr valuesToCopy = nullptr);
};
}