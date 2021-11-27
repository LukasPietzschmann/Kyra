#include "TypeChecker.hpp"

#include <cassert>
#include <memory>
#include <string>
#include <unordered_map>

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
#include "../Values/Value.hpp"
#include "../Variable.hpp"
#include "ClassType.hpp"
#include "FunctionType.hpp"
#include "NativeTypes.hpp"

namespace Slanguage {
TypeChecker& TypeChecker::getInstance() {
	static TypeChecker instance;
	return instance;
}

TypeChecker::Result TypeChecker::check(const std::vector<Statement::Ptr>& statements) {
	TypeChecker::Result result;
	for(const auto& statement : statements)
		check(statement);

	for(const auto& error : m_errors)
		result.insertError(error);

	m_errors.clear();
	return result;
}

void TypeChecker::check(const Statement::Ptr& statement) { statement->accept(*this); }

template <class Callback>
TypeContext::Ptr TypeChecker::runInNewContext(const Callback& function,
		TypeContext::Ptr parent,
		TypeContext::Ptr valuesToCopy) {
	TypeContext::Ptr contextCopy = TypeContext::makePtr<TypeContext>(*m_currentContext);
	m_currentContext = TypeContext::makePtr<TypeContext>(parent);
	if(valuesToCopy != nullptr) {
		for(const auto& [name, variable] : valuesToCopy->getVariables())
			m_currentContext->declareVar(name, variable);
		for(const auto& [typeName, type] : valuesToCopy->getTypes())
			m_currentContext->declareType(typeName, type);
	}
	function();
	TypeContext::Ptr modifiedContext = m_currentContext;
	m_currentContext = contextCopy;
	return modifiedContext;
}

void TypeChecker::visitAccessExpr(AccessExpr& accessExpr) {
	EXPR_ACCEPT(accessExpr.getOwner(), *this, Type::Ptr ownerType);
	const std::string& name = accessExpr.getName().getValue().asString();
	const auto& result = ownerType->knowsAbout(name);
	if(!result.has_value())
		THROW_TYPING_ERROR(UndefinedMemberError(accessExpr.getPosition(), ownerType->getName(), name));
	EXPR_RETURN_FROM_VISIT(result->value);
}

void TypeChecker::visitAssignmentExpr(AssignmentExpr& assignmentExpr) {
	Type::Ptr assignedValueType;
	const std::string& name = assignmentExpr.getName().getValue().asString();

	if(assignmentExpr.getOwner() != nullptr) {
		EXPR_ACCEPT(assignmentExpr.getOwner(), *this, Type::Ptr ownerType);
		assignedValueType = ownerType->knowsAbout(name)->value;
	} else
		assignedValueType = m_currentContext->getVar(name)->value;

	if(assignedValueType == nullptr)
		THROW_TYPING_ERROR(UndefinedVariableError(assignmentExpr.getPosition(), name));
	if(!m_currentContext->getVar(name)->isMutable)
		THROW_TYPING_ERROR(AssignmentToConstError(assignmentExpr.getPosition(), name));
	EXPR_ACCEPT(assignmentExpr.getNewValue(), *this, Type::Ptr newValueType);
	if(!newValueType->canBeAssignedTo(assignedValueType))
		THROW_TYPING_ERROR(
				WrongTypeError(assignmentExpr.getPosition(), assignedValueType->getName(), newValueType->getName()));
	EXPR_RETURN_FROM_VISIT(newValueType);
}

// TODO: It should be possible to override specific slg-functions to implement those oprrators
void TypeChecker::visitBinaryExpr(BinaryExpr& binaryExpr) {
	const Type::Ptr& num = NativeTypes::make(Value::NativeTypes::Number);
	const Type::Ptr& str = NativeTypes::make(Value::NativeTypes::String);
	const Type::Ptr& boolean = NativeTypes::make(Value::NativeTypes::Bool);
	const Token& oper = binaryExpr.getOperator();
	EXPR_ACCEPT(binaryExpr.getLhs(), *this, Type::Ptr lhs);
	EXPR_ACCEPT(binaryExpr.getRhs(), *this, Type::Ptr rhs);

	switch(oper.getType()) {
		case TokenType::EQUAL_EQUAL:
		case TokenType::BANG_EQUAL:
		case TokenType::GREATER:
		case TokenType::GREATER_EQUAL:
		case TokenType::LESS:
		case TokenType::LESS_EQUAL: EXPR_RETURN_FROM_VISIT(boolean);
		case TokenType::SLASH:
		case TokenType::MINUS:
			if(*lhs == num && *rhs == num)
				EXPR_RETURN_FROM_VISIT(num);
			THROW_TYPING_ERROR(
					UnsupportedOperator(binaryExpr.getPosition(), lhs->getName(), oper.getLexeme(), rhs->getName()));
		case TokenType::PLUS:
			if(*lhs == num && *rhs == num)
				EXPR_RETURN_FROM_VISIT(num);
			if(*lhs == str && *rhs == str)
				EXPR_RETURN_FROM_VISIT(str);
			THROW_TYPING_ERROR(
					UnsupportedOperator(binaryExpr.getPosition(), lhs->getName(), oper.getLexeme(), rhs->getName()));
		case TokenType::STAR:
			if(*lhs == num && *rhs == num)
				EXPR_RETURN_FROM_VISIT(num);
			if(*lhs == str && *rhs == num)
				EXPR_RETURN_FROM_VISIT(str);
			THROW_TYPING_ERROR(
					UnsupportedOperator(binaryExpr.getPosition(), lhs->getName(), oper.getLexeme(), rhs->getName()));
		default:
			THROW_TYPING_ERROR(
					UnsupportedOperator(binaryExpr.getPosition(), lhs->getName(), oper.getLexeme(), rhs->getName()));
	}
}

void TypeChecker::visitCallExpr(CallExpr& callExpr) {
	EXPR_ACCEPT(callExpr.getFunction(), *this, Type::Ptr gFunction);
	const auto& function = std::dynamic_pointer_cast<FunctionType>(gFunction);
	if(gFunction == nullptr)
		return;
	if(function == nullptr)
		THROW_TYPING_ERROR(WrongTypeError(callExpr.getPosition(), Value::NativeTypes::Function, gFunction->getName()));
	if(function->getParameters().size() != callExpr.getArguments().size())
		THROW_TYPING_ERROR(ArityError(callExpr.getPosition(),
				function->getArity(),
				callExpr.getArguments().size(),
				function->getName()));
	for(unsigned long i = 0; i < function->getParameters().size(); ++i) {
		const Type::Ptr param = function->getParameters()[i];
		EXPR_ACCEPT(callExpr.getArguments()[i], *this, Type::Ptr arg);
		if(!arg->canBeAssignedTo(param))
			THROW_TYPING_ERROR(WrongTypeError(callExpr.getPosition(), param->getName(), arg->getName()));
	}

	EXPR_RETURN_FROM_VISIT(function->getReturnType());
}

void TypeChecker::visitFunction(FunctionExpr& functionExpr) {
	FunctionType* enclosingFunctionType = m_currentFunction;
	EXPR_ACCEPT(functionExpr.getReturnType(), *this, Type::Ptr returnType);
	std::vector<Type::Ptr> parameters;
	TypeContext::Ptr paramScope = runInNewContext(
			[&functionExpr, &parameters, this]() {
				for(const auto& param : functionExpr.getParameters()) {
					const auto& result = m_currentContext->getType(param.type);
					if(!result.has_value())
						THROW_TYPING_ERROR(UndefinedTypeError(functionExpr.getPosition(), param.type));
					if(!result.value()->isApplicableForDeclaration())
						THROW_TYPING_ERROR(UndefinedTypeError(functionExpr.getPosition(), result.value()->getName()));
					parameters.push_back(result.value());
					if(!m_currentContext->declareVar(param.name.getValue().asString(), result.value(), true))
						THROW_TYPING_ERROR(AlreadyDefinedVariableError(functionExpr.getPosition(),
								param.name.getValue().asString()));
				}
			},
			m_currentContext);

	Type::Ptr functionType = Type::makePtr<FunctionType>(returnType, parameters);
	m_currentFunction = std::dynamic_pointer_cast<FunctionType>(functionType).get();

	runInNewContext([&functionExpr, this]() { functionExpr.getImplementation()->accept(*this); },
			m_currentContext,
			paramScope);

	if(!m_doesCurrentFunctionReturn && m_currentFunction->getReturnType()->getName() != Value::NativeTypes::Nothing)
		THROW_TYPING_ERROR(WrongTypeError(functionExpr.getPosition(),
				m_currentFunction->getReturnType()->getName(),
				Value::NativeTypes::Nothing));

	m_doesCurrentFunctionReturn = false;
	m_currentFunction = enclosingFunctionType;
	EXPR_RETURN_FROM_VISIT(functionType);
}

void TypeChecker::visitGroupExpr(GroupExpr& groupExpr) {
	EXPR_ACCEPT(groupExpr.getExpr(), *this, Type::Ptr type);
	EXPR_RETURN_FROM_VISIT(type);
}

void TypeChecker::visitInstantiationExpr(InstantiationExpr& instantiationExpr) {
	const auto& result = m_currentContext->getType(instantiationExpr.getName());
	if(!result.has_value())
		THROW_TYPING_ERROR(UndefinedTypeError(instantiationExpr.getPosition(), instantiationExpr.getName()));
	auto classType = std::dynamic_pointer_cast<ClassType>(result.value());
	if(instantiationExpr.getArguments().size() != classType->getArity())
		THROW_TYPING_ERROR(ArityError(instantiationExpr.getPosition(),
				classType->getArity(),
				instantiationExpr.getArguments().size(),
				"Constructor of class " + instantiationExpr.getName()));
	for(unsigned long i = 0; i < classType->getArity(); ++i) {
		const Type::Ptr& expected = classType->getConstructorParameter()[i];
		EXPR_ACCEPT(instantiationExpr.getArguments()[i], *this, Type::Ptr provided);
		if(*expected != provided)
			THROW_TYPING_ERROR(
					WrongTypeError(instantiationExpr.getPosition(), expected->getName(), provided->getName()));
	}
	EXPR_RETURN_FROM_VISIT(result.value());
}

void TypeChecker::visitLiteral(LiteralExpr& literalExpr) {
	EXPR_RETURN_FROM_VISIT(m_currentContext->getType(literalExpr.getValue()->getType()).value());
}

void TypeChecker::visitTypeExpr(TypeExpr& typeExpr) {
	if(typeExpr.isFunction()) {
		EXPR_ACCEPT(typeExpr.getReturnType(), *this, Type::Ptr returnType);
		std::vector<Type::Ptr> paramTypes;
		for(const auto& param : typeExpr.getParameterTypes()) {
			EXPR_ACCEPT(param, *this, Type::Ptr paramType);
			paramTypes.push_back(paramType);
		}
		EXPR_RETURN_FROM_VISIT(Type::makePtr<FunctionType>(returnType, paramTypes));
	}
	const auto& result = m_currentContext->getType(typeExpr.getName());
	if(!result.has_value())
		THROW_TYPING_ERROR(UndefinedTypeError(typeExpr.getPosition(), typeExpr.getName()));
	EXPR_RETURN_FROM_VISIT(result.value());
}

void TypeChecker::visitUnaryExpr(UnaryExpr& unaryExpr) {
	// TODO check for function in type. This allows for overriding those functions
	const TokenType& oper = unaryExpr.getOperator().getType();
	EXPR_ACCEPT(unaryExpr.getRhs(), *this, Type::Ptr rhs);
	switch(oper) {
		case TokenType::BANG: EXPR_RETURN_FROM_VISIT(m_currentContext->getType(Value::NativeTypes::Bool).value());
		case TokenType::MINUS:
			if(rhs->getName() == Value::NativeTypes::Number)
				EXPR_RETURN_FROM_VISIT(NativeTypes::make(Value::NativeTypes::Number));
			else
				THROW_TYPING_ERROR(UnsupportedOperator(unaryExpr.getPosition(),
						unaryExpr.getOperator().getValue().asString(),
						rhs->getName()));
		default: assert(false);
	}
}

void TypeChecker::visitVariable(VariableExpr& variableExpr) {
	const std::string& name = variableExpr.getName().getValue().asString();
	const auto& result = m_currentContext->getVar(name);
	if(!result.has_value()) {
		if(m_currentClassName != nullptr)
			THROW_TYPING_ERROR(UndefinedMemberError(variableExpr.getPosition(), std::string(m_currentClassName), name));
		else
			THROW_TYPING_ERROR(UndefinedVariableError(variableExpr.getPosition(), name));
	}
	EXPR_RETURN_FROM_VISIT(result->value);
}

void TypeChecker::visitBlockStmt(BlockStmt& blockStmt) {
	runInNewContext(
			[this, &blockStmt]() {
				for(const auto& statement : blockStmt.getStatements())
					statement->accept(*this);
			},
			m_currentContext);
}

void TypeChecker::visitDeclarationStmt(DeclarationStmt& declarationStmt) {
	const std::string& name = declarationStmt.getIdentifier().getValue().asString();

	Type::Ptr expectedType{};
	if(declarationStmt.getType() == nullptr) {
		EXPR_ACCEPT(declarationStmt.getInitializer(), *this, expectedType);
	} else {
		EXPR_ACCEPT(declarationStmt.getType(), *this, expectedType);
		if(!expectedType->isApplicableForDeclaration())
			THROW_TYPING_ERROR(UndefinedTypeError(declarationStmt.getPosition(), expectedType->getName()));
		if(declarationStmt.getInitializer() != nullptr) {
			// TODO: this is necessary for recursion to work
			// Is types aren't explicitly annotated, this code won't run
			if(expectedType->isFunction())
				m_currentContext->declareVar(name, expectedType, declarationStmt.isMutable());
			EXPR_ACCEPT(declarationStmt.getInitializer(), *this, Type::Ptr initType);
			if(expectedType->isFunction())
				m_currentContext->removeVar(name);
			if(!initType->canBeAssignedTo(expectedType))
				THROW_TYPING_ERROR(
						WrongTypeError(declarationStmt.getPosition(), expectedType->getName(), initType->getName()));
		}
	}
	if(!m_currentContext->declareVar(name, expectedType, declarationStmt.isMutable())) {
		if(m_currentClassName != nullptr)
			THROW_TYPING_ERROR(
					AlreadyDefinedMemberError(declarationStmt.getPosition(), std::string(m_currentClassName), name));
		else
			THROW_TYPING_ERROR(AlreadyDefinedVariableError(declarationStmt.getPosition(), name));
	}
}

void TypeChecker::visitClassDeclarationStmt(ClassDeclarationStmt& classDeclarationStmt) {
	m_currentClassName = classDeclarationStmt.getIdentifier().getValue().asString().data();
	const std::string& name = classDeclarationStmt.getIdentifier().getValue().asString();
	if(m_currentContext->getType(name).has_value())
		THROW_TYPING_ERROR(AlreadyDefinedTypeError(classDeclarationStmt.getPosition(), name));

	std::vector<Type::Ptr> constructorParams;
	TypeContext::Ptr paramScope = runInNewContext(
			[&classDeclarationStmt, &constructorParams, this]() {
				for(const auto& param : classDeclarationStmt.getConstructorParameters()) {
					const auto& result = m_currentContext->getType(param.type);
					if(!result.has_value())
						THROW_TYPING_ERROR(UndefinedTypeError(classDeclarationStmt.getPosition(), param.type));
					if(!result.value()->isApplicableForDeclaration())
						THROW_TYPING_ERROR(
								UndefinedTypeError(classDeclarationStmt.getPosition(), result.value()->getName()));
					constructorParams.push_back(result.value());
					if(!m_currentContext->declareVar(param.name.getValue().asString(), result.value(), true))
						THROW_TYPING_ERROR(AlreadyDefinedVariableError(classDeclarationStmt.getPosition(),
								param.name.getValue().asString()));
				}
			},
			m_currentContext);

	TypeContext::Ptr declScope = runInNewContext(
			[&classDeclarationStmt, this]() {
				for(const auto& decl : classDeclarationStmt.getDeclarations())
					decl->accept(*this);
			},
			m_currentContext,
			paramScope);

	m_currentContext->declareType(name, Type::makePtr<ClassType>(name, declScope->getVariables(), constructorParams));
	m_currentClassName = nullptr;
}
void TypeChecker::visitExpressionStmt(ExpressionStmt& expressionStmt) { expressionStmt.getExpr()->accept(*this); }

void TypeChecker::visitPrintStmt(PrintStmt& printStmt) { EXPR_ACCEPT(printStmt.getExpr(), *this, Type::Ptr type); }

void TypeChecker::visitReturnStmt(ReturnStmt& returnStmt) {
	if(m_currentFunction == nullptr)
		THROW_TYPING_ERROR(InvalidReturnError(returnStmt.getPosition()));
	EXPR_ACCEPT(returnStmt.getExpr(), *this, Type::Ptr returnedType);
	if(*returnedType != m_currentFunction->getReturnType())
		THROW_TYPING_ERROR(WrongTypeError(returnStmt.getPosition(),
				m_currentFunction->getReturnType()->getName(),
				returnedType->getName()));
	m_doesCurrentFunctionReturn = true;
}
}