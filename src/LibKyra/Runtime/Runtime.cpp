#include "Runtime.hpp"

#include <cassert>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "../Exceptions.hpp"
#include "../Expressions/AccessExpr.hpp"
#include "../Expressions/AssignmentExpr.hpp"
#include "../Expressions/BinaryExpr.hpp"
#include "../Expressions/CallExpr.hpp"
#include "../Expressions/GroupExpr.hpp"
#include "../Expressions/InstantiationExpr.hpp"
#include "../Expressions/LiteralExpr.hpp"
#include "../Expressions/UnaryExpr.hpp"
#include "../Expressions/VariableExpr.hpp"
#include "../Statements/BlockStmt.hpp"
#include "../Statements/ClassDeclarationStmt.hpp"
#include "../Statements/DeclarationStmt.hpp"
#include "../Statements/ExpressionStmt.hpp"
#include "../Statements/PrintStmt.hpp"
#include "../Statements/ReturnStmt.hpp"
#include "../Statements/WhileStmt.hpp"
#include "../Token.hpp"
#include "../TokenType.hpp"
#include "../Values/Bool.hpp"
#include "../Values/Function.hpp"
#include "../Values/Klass.hpp"
#include "../Values/Nothing.hpp"
#include "../Values/Number.hpp"
#include "../Values/Value.hpp"

namespace Kyra {
class FunctionExpr;
class TypeExpr;
Runtime& Runtime::getInstance() {
	static Runtime instance;
	return instance;
}

void Runtime::executeStatement(Statement::Ptr statement, RuntimeContext::Ptr contextToExecuteOn) {
	if(contextToExecuteOn == nullptr) {
		statement->accept(*this);
		return;
	}

	RuntimeContext::Ptr contextCopy = RuntimeContext::makePtr<RuntimeContext>(*m_currentContext);
	m_currentContext = contextToExecuteOn;
	statement->accept(*this);
	m_currentContext = contextCopy;
}

Value::Ptr Runtime::executeExpression(Expression::Ptr expression, RuntimeContext::Ptr contextToExecuteOn) {
	if(contextToExecuteOn == nullptr) {
		EXPR_ACCEPT(expression, *this, Value::Ptr result);
		return result;
	}

	RuntimeContext::Ptr contextCopy = RuntimeContext::makePtr<RuntimeContext>(*m_currentContext);
	m_currentContext = contextToExecuteOn;
	EXPR_ACCEPT(expression, *this, Value::Ptr result);
	m_currentContext = contextCopy;
	return result;
}

void Runtime::visitAccessExpr(AccessExpr& accessExpr) {
	EXPR_ACCEPT(accessExpr.getOwner(), *this, Value::Ptr owner);
	const auto& klass = Value::as<Klass>(owner);
	const auto& var = klass->getInstanceContext()->getVar(accessExpr.getName().getValue().asString());
	if(!var.has_value())
		assert(false);
	EXPR_RETURN_FROM_VISIT(var->value);
}

void Runtime::visitAssignmentExpr(AssignmentExpr& assignmentExpr) {
	EXPR_ACCEPT(assignmentExpr.getNewValue(), *this, Value::Ptr newValue);
	if(!m_currentContext->mutateVar(assignmentExpr.getName().getValue().asString(), newValue))
		assert(false);
	EXPR_RETURN_FROM_VISIT(newValue);
}

void Runtime::visitBinaryExpr(BinaryExpr& binaryExpr) {
	EXPR_ACCEPT(binaryExpr.getLhs(), *this, Value::Ptr lhs);
	EXPR_ACCEPT(binaryExpr.getRhs(), *this, Value::Ptr rhs);
	if(!lhs->isNative()) {
		const auto& gFunction =
				Value::as<Klass>(lhs)->getDeclaration("operator" + binaryExpr.getOperator().getLexeme());
		auto function = Value::as<Function>(gFunction);
		const auto result = function->exec({rhs});
		EXPR_RETURN_FROM_VISIT(result);
	}
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
	const auto& typeOrError = m_currentContext->getType(instantiationExpr.getName());
	if(!typeOrError.has_value())
		assert(false);
	Value::Ptr klass = typeOrError.value();
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
	const auto& var = m_currentContext->getVar(variableExpr.getName().getValue().asString());
	if(!var.has_value())
		assert(false);
	EXPR_RETURN_FROM_VISIT(var->value);
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
	if(!m_currentContext->declareVar(name, init, declarationStmt.isMutable()))
		assert(false);
}

void Runtime::visitClassDeclarationStmt(ClassDeclarationStmt& classDeclarationStmt) {
	const std::string& name = classDeclarationStmt.getIdentifier().getValue().asString();
	if(!m_currentContext->declareType(name, Value::makePtr<Klass>(classDeclarationStmt)))
		assert(false);
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

void Runtime::visitWhileStmt(WhileStmt& whileStmt) {
	EXPR_ACCEPT(whileStmt.getCondition(), *this, Value::Ptr condition);
	while(condition->isImplicitlyTrue()) {
		whileStmt.getStatement()->accept(*this);
		EXPR_ACCEPT(whileStmt.getCondition(), *this, condition);
	}
}

template <typename Callback>
RuntimeContext::Ptr Runtime::runInNewContext(const Callback& callback, RuntimeContext::Ptr parent) {
	RuntimeContext::Ptr contextCopy = RuntimeContext::makePtr<RuntimeContext>(*m_currentContext);
	m_currentContext = RuntimeContext::makePtr<RuntimeContext>(parent);
	callback();
	RuntimeContext::Ptr modifiedContext = m_currentContext;
	return modifiedContext;
}
}