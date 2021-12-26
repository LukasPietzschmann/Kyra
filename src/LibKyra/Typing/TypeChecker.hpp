#pragma once

#include <iosfwd>
#include <iostream>
#include <optional>
#include <type_traits>
#include <utility>
#include <vector>

#include "../Expressions/Expression.hpp"
#include "../HasPtrAlias.hpp"
#include "../Statements/Statement.hpp"
#include "Type.hpp"
#include "TypeContext.hpp"
#include "TypingErrors.hpp"

namespace Kyra {
class AccessExpr;
class AssignmentExpr;
class BinaryExpr;
class BlockStmt;
class CallExpr;
class ClassDeclarationStmt;
class DeclarationStmt;
class ExpressionStmt;
class FunctionExpr;
class FunctionType;
class GroupExpr;
class InstantiationExpr;
class LiteralExpr;
class PrintStmt;
class ReturnStmt;
class TypeExpr;
class UnaryExpr;
class VariableExpr;

class TypeReprHelper {
public:
	TypeReprHelper() = default;
	TypeReprHelper(const Type::Repr& repr) : repr(repr) {}

	TypeReprHelper& operator=(const Type::Repr& r) {
		repr = r;
		return *this;
	}

	Type::Ptr operator->() const { return TypeProvider::the().decode(repr); }
	Type::Ptr operator*() const { return TypeProvider::the().decode(repr); }

	bool operator==(Type::Ptr type) const { return **this == type; }
	bool operator==(const TypeReprHelper& type) const { return **this == *type; }

	const Type::Repr& getRepr() const { return repr; }

	bool hasValue() const { return !repr.empty(); }

private:
	Type::Repr repr;
};

class TypeChecker : public ExpressionVisitor, public StatementVisitor {
#define THROW_TYPING_ERROR(error)               \
	do {                                        \
		m_errors.push_back((error).getCause()); \
		return;                                 \
	} while(0)

	EXPR_NEEDS_VISIT_RETURN_OF_TYPE(TypeReprHelper);

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
	TypeContext::Ptr m_currentContext{TypeContext::makePtr<TypeContext>()};
	FunctionType* m_currentFunction{};
	bool m_doesCurrentFunctionReturn{};
	char* m_currentClassName{};

	void check(const Statement::Ptr& statement);
	template <class Callback>
	TypeContext::Ptr runInNewContext(const Callback& function,
			TypeContext::Ptr parent,
			TypeContext::Ptr valuesToCopy = nullptr);
};
}