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
#include "TypingErrors.hpp"

namespace LibSlg {
class TypeChecker : public ExpressionVisitor, public StatementVisitor {
#define THROW_TYPING_ERROR(error)               \
	do {                                        \
		m_errors.push_back((error).getCause()); \
		return;                                 \
	} while(0)
private:
	class Scope {
	public:
		explicit Scope(Scope* parent) : m_parent(parent), m_variables({}), m_types({}) {
			for(const auto& nativeType : Value::NativeTypes::All)
				m_types.emplace(nativeType, NativeTypes::make(nativeType));
		}
		~Scope() { delete m_parent; }

		Scope() : Scope(nullptr) {}

		std::optional<Type::Ptr> getType(const std::string& name) {
			const auto& it = m_types.find(name);
			if(it != m_types.end())
				return it->second;
			if(m_parent != nullptr)
				return m_parent->getType(name);
			return {};
		}

		std::optional<Variable> getVar(const std::string& name) {
			const auto& it = m_variables.find(name);
			if(it != m_variables.end())
				return it->second;
			if(m_parent != nullptr)
				return m_parent->getVar(name);
			return {};
		}

		bool setType(const std::string& name, const Type::Ptr& type) {
			if(m_types.contains(name))
				return false;
			m_types.emplace(name, type);
			return true;
		}

		bool setVar(const std::string& name, const Type::Ptr& varType, bool isMutable) {
			return setVar(name, Variable(varType, isMutable));
		}

		bool setVar(const std::string& name, const Variable& var) {
			if(m_variables.contains(name))
				return false;
			m_variables.emplace(name, var);
			return true;
		}

		const std::unordered_map<std::string, Variable>& getVariables() const { return m_variables; }
		const std::unordered_map<std::string, Type::Ptr>& getTypes() const { return m_types; }

	private:
		Scope* m_parent;
		std::unordered_map<std::string, Variable> m_variables;
		std::unordered_map<std::string, Type::Ptr> m_types;
	};

	EXPR_NEEDS_VISIT_RETURN_OF_TYPE(Type::Ptr);
	STMT_NEEDS_VISIT_RETURN_OF_TYPE(Scope*);

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
	~TypeChecker() { delete m_currentScope; }
	TypeChecker(TypeChecker const&) = delete;
	void operator=(TypeChecker const&) = delete;

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
	TypeChecker() : m_currentScope(new Scope(nullptr)){};

	std::vector<std::string> m_errors;
	Scope* m_currentScope;
	FunctionType* m_currentFunction{};
	bool m_doesCurrentFunctionReturn{};
	char* m_currentClassName{};

	void check(const Statement::Ptr& statement);
	Scope* runInNewScope(const std::function<void()>& function, Scope* parent, Scope* valuesToCopy = nullptr);
};
}