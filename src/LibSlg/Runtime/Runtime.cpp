#include "Runtime.hpp"

#include <cassert>
#include <iostream>
#include <memory>
#include <type_traits>
#include <vector>

#include "../Exceptions.hpp"
#include "../Expressions/AccessExpr.hpp"
#include "../Expressions/AssignmentExpr.hpp"
#include "../Expressions/BinaryExpr.hpp"
#include "../Expressions/CallExpr.hpp"
#include "../Expressions/Expression.hpp"
#include "../Expressions/GroupExpr.hpp"
#include "../Expressions/InstantiationExpr.hpp"
#include "../Expressions/LiteralExpr.hpp"
#include "../Expressions/UnaryExpr.hpp"
#include "../Expressions/VariableExpr.hpp"
#include "../HasPtrAlias.hpp"
#include "../Statements/BlockStmt.hpp"
#include "../Statements/ClassDeclarationStmt.hpp"
#include "../Statements/DeclarationStmt.hpp"
#include "../Statements/ExpressionStmt.hpp"
#include "../Statements/PrintStmt.hpp"
#include "../Statements/ReturnStmt.hpp"
#include "../Statements/Statement.hpp"
#include "../Token.hpp"
#include "../TokenType.hpp"
#include "../Values/Bool.hpp"
#include "../Values/Function.hpp"
#include "../Values/Klass.hpp"
#include "../Values/Nothing.hpp"
#include "../Values/Number.hpp"
#include "../Values/Value.hpp"
#include "../Variable.hpp"

namespace Slanguage {
class FunctionExpr;
class TypeExpr;
Runtime& Runtime::getInstance() {
	static Runtime instance;
	return instance;
}

void Runtime::executeStatement(Statement::WeakPtr statement, RuntimeContext::Ptr contextToExecuteOn) {
	if(contextToExecuteOn == nullptr) {
		Statement::lock(statement)->accept(*this);
		return;
	}

	RuntimeContext::Ptr contextCopy = RuntimeContext::makePtr<RuntimeContext>(*m_currentContext);
	m_currentContext = contextToExecuteOn;
	Statement::lock(statement)->accept(*this);
	m_currentContext = contextCopy;
}

Value::Ptr Runtime::executeExpression(Expression::WeakPtr expression, RuntimeContext::Ptr contextToExecuteOn) {
	if(contextToExecuteOn == nullptr) {
		EXPR_ACCEPT(Expression::lock(expression), *this, Value::WeakPtr result);
		return Value::lock(result);
	}

	RuntimeContext::Ptr contextCopy = RuntimeContext::makePtr<RuntimeContext>(*m_currentContext);
	m_currentContext = contextToExecuteOn;
	EXPR_ACCEPT(Expression::lock(expression), *this, Value::WeakPtr result);
	m_currentContext = contextCopy;
	return Value::lock(result);
}

void Runtime::visitAccessExpr(AccessExpr& accessExpr) {
	EXPR_ACCEPT(accessExpr.getOwner(), *this, Value::Ptr owner);
	const auto& klass = Value::as<Klass>(owner);
	EXPR_RETURN_FROM_VISIT(klass->getInstanceContext()->getVar(accessExpr.getName().getValue().asString()).value);
}

void Runtime::visitAssignmentExpr(AssignmentExpr& assignmentExpr) {
	EXPR_ACCEPT(assignmentExpr.getNewValue(), *this, Value::Ptr newValue);
	m_currentContext->mutate(assignmentExpr.getName().getValue().asString(), newValue);
	EXPR_RETURN_FROM_VISIT(newValue);
}

void Runtime::visitBinaryExpr(BinaryExpr& binaryExpr) {
	EXPR_ACCEPT(binaryExpr.getLhs(), *this, Value::Ptr lhs);
	EXPR_ACCEPT(binaryExpr.getRhs(), *this, Value::Ptr rhs);
	switch(binaryExpr.getOperator().getType()) {
		case TokenType::EQUAL_EQUAL: EXPR_RETURN_FROM_VISIT(Value::makePtr<Bool>(*lhs == rhs));
		case TokenType::BANG_EQUAL: EXPR_RETURN_FROM_VISIT(Value::makePtr<Bool>(*lhs != rhs));
		case TokenType::GREATER: EXPR_RETURN_FROM_VISIT(Value::makePtr<Bool>(*lhs > rhs));
		case TokenType::GREATER_EQUAL: EXPR_RETURN_FROM_VISIT(Value::makePtr<Bool>(*lhs >= rhs));
		case TokenType::LESS: EXPR_RETURN_FROM_VISIT(Value::makePtr<Bool>(*lhs < rhs));
		case TokenType::LESS_EQUAL: EXPR_RETURN_FROM_VISIT(Value::makePtr<Bool>(*lhs <= rhs));
		case TokenType::MINUS: EXPR_RETURN_FROM_VISIT(*lhs - rhs);
		case TokenType::PLUS: EXPR_RETURN_FROM_VISIT(*lhs + rhs);
		case TokenType::STAR: EXPR_RETURN_FROM_VISIT(*lhs * rhs);
		case TokenType::SLASH: EXPR_RETURN_FROM_VISIT(*lhs / rhs);
		default: assert(false);
	}
}

void Runtime::visitCallExpr(CallExpr& callExpr) {
	EXPR_ACCEPT(callExpr.getFunction(), *this, Value::Ptr gFunction);
	auto function = Value::as<Function>(gFunction);
	assert(callExpr.getArguments().size() == function->getArity());

	std::vector<Value::Ptr> args;
	for(const auto& arg : callExpr.getArguments()) {
		EXPR_ACCEPT(arg, *this, Value::Ptr value);
		args.push_back(value);
	}
	Value::Ptr returnVal = function->exec(args);
	EXPR_RETURN_FROM_VISIT(returnVal);
}

void Runtime::visitFunction(FunctionExpr& functionExpr) {
	EXPR_RETURN_FROM_VISIT(Value::makePtr<Function>(functionExpr, m_currentContext));
}

void Runtime::visitGroupExpr(GroupExpr& groupExpr) {
	EXPR_ACCEPT(groupExpr.getExpr(), *this, Value::Ptr value);
	EXPR_RETURN_FROM_VISIT(value);
}

void Runtime::visitInstantiationExpr(InstantiationExpr& instantiationExpr) {
	Value::Ptr klass = m_currentContext->getCustomType(instantiationExpr.getName());
	std::vector<Value::Ptr> args;
	for(const auto& arg : instantiationExpr.getArguments()) {
		EXPR_ACCEPT(arg, *this, Value::Ptr value);
		args.emplace_back(value);
	}
	Value::as<Klass>(klass)->instantiate(args);
	EXPR_RETURN_FROM_VISIT(klass);
}

void Runtime::visitLiteral(LiteralExpr& literalExpr) { EXPR_RETURN_FROM_VISIT(literalExpr.getValue()); }

void Runtime::visitTypeExpr(TypeExpr&) {
	// a type expr. has no effect during runtime
}

void Runtime::visitUnaryExpr(UnaryExpr& unaryExpr) {
	EXPR_ACCEPT(unaryExpr.getRhs(), *this, Value::Ptr value);
	switch(unaryExpr.getOperator().getType()) {
		case TokenType::BANG: EXPR_RETURN_FROM_VISIT(Value::makePtr<Bool>(!value->isImplicitlyTrue()));
		case TokenType::MINUS: EXPR_RETURN_FROM_VISIT(*value * Value::makePtr<Number>(-1));
		default: assert(false);
	}
}

void Runtime::visitVariable(VariableExpr& variableExpr) {
	EXPR_RETURN_FROM_VISIT(m_currentContext->getVar(variableExpr.getName().getValue().asString()).value);
}

void Runtime::visitBlockStmt(BlockStmt& blockStmt) {
	runInNewContext(
			[&blockStmt, this]() {
				for(const auto& statement : blockStmt.getStatements())
					statement->accept(*this);
			},
			m_currentContext);
}

void Runtime::visitDeclarationStmt(DeclarationStmt& declarationStmt) {
	Value::Ptr init = Value::makePtr<Nothing>();
	if(declarationStmt.getInitializer() != nullptr) {
		EXPR_ACCEPT(declarationStmt.getInitializer(), *this, init);
	}

	const std::string& name = declarationStmt.getIdentifier().getValue().asString();
	m_currentContext->declareVar(name, init, declarationStmt.isMutable());
}

void Runtime::visitClassDeclarationStmt(ClassDeclarationStmt& classDeclarationStmt) {
	const std::string& name = classDeclarationStmt.getIdentifier().getValue().asString();
	m_currentContext->declareType(name, Value::makePtr<Klass>(classDeclarationStmt));
}

void Runtime::visitExpressionStmt(ExpressionStmt& expressionStmt) { expressionStmt.getExpr()->accept(*this); }

void Runtime::visitPrintStmt(PrintStmt& printStmt) {
	EXPR_ACCEPT(printStmt.getExpr(), *this, Value::Ptr value);
	std::cout << value->toString() << std::endl;
}

void Runtime::visitReturnStmt(ReturnStmt& returnStmt) {
	EXPR_ACCEPT(returnStmt.getExpr(), *this, Value::Ptr value);
	throw ReturnException(value);
}

template <typename Callback>
RuntimeContext::Ptr Runtime::runInNewContext(const Callback& callback, RuntimeContext::Ptr parent) {
	RuntimeContext::Ptr contextCopy = RuntimeContext::makePtr<RuntimeContext>(*m_currentContext);
	m_currentContext = RuntimeContext::makePtr<RuntimeContext>(parent);
	callback();
	RuntimeContext::Ptr modifiedContext = m_currentContext;
	m_currentContext = contextCopy;
	return modifiedContext;
}
}