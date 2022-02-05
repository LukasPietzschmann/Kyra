#include "TypeChecker.hpp"

#include <string>

#include "../Expressions/AccessExpr.hpp"
#include "../Expressions/AssignmentExpr.hpp"
#include "../Expressions/BinaryExpr.hpp"
#include "../Expressions/CallExpr.hpp"
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
#include "../Statements/WhileStmt.hpp"
#include "../Variable.hpp"
#include "ClassType.hpp"
#include "FunctionType.hpp"

namespace Kyra {
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
	EXPR_ACCEPT(accessExpr.getOwner(), *this, TypeReprHelper ownerType);
	const std::string& name = accessExpr.getName().getValue().asString();
	const auto& result = ownerType->knowsAbout(name);
	if(!result.has_value())
		THROW_TYPING_ERROR(UndefinedMemberError(accessExpr.getPosition(), ownerType->getName(), name));
	EXPR_RETURN_FROM_VISIT(result->value);
}

void TypeChecker::visitAssignmentExpr(AssignmentExpr& assignmentExpr) {
	TypeReprHelper assignedValueType;
	const std::string& name = assignmentExpr.getName().getValue().asString();

	if(assignmentExpr.getOwner() != nullptr) {
		EXPR_ACCEPT(assignmentExpr.getOwner(), *this, TypeReprHelper ownerType);
		assignedValueType = ownerType->knowsAbout(name)->value;
	} else
		assignedValueType = m_currentContext->getVar(name)->value;

	if(!assignedValueType.hasValue())
		THROW_TYPING_ERROR(UndefinedVariableError(assignmentExpr.getPosition(), name));
	if(!m_currentContext->getVar(name)->isMutable)
		THROW_TYPING_ERROR(AssignmentToConstError(assignmentExpr.getPosition(), name));
	EXPR_ACCEPT(assignmentExpr.getNewValue(), *this, TypeReprHelper newValueType);
	if(!newValueType->canBeAssignedTo(*assignedValueType))
		THROW_TYPING_ERROR(
				WrongTypeError(assignmentExpr.getPosition(), assignedValueType->getName(), newValueType->getName()));
	EXPR_RETURN_FROM_VISIT(newValueType);
}

void TypeChecker::visitBinaryExpr(BinaryExpr& binaryExpr) {
	EXPR_ACCEPT(binaryExpr.getLhs(), *this, TypeReprHelper lhs);
	EXPR_ACCEPT(binaryExpr.getRhs(), *this, TypeReprHelper rhs);

	const std::string& expectedFunctionName = "operator" + binaryExpr.getOperator().getLexeme();
	if(const auto& res = lhs->knowsAbout(expectedFunctionName); res.has_value()) {
		TypeReprHelper type = res->value;
		if(type->canBeCalledWith({rhs.getRepr()}))
			EXPR_RETURN_FROM_VISIT(std::dynamic_pointer_cast<FunctionType>(*type)->getReturnType());
	}
	THROW_TYPING_ERROR(UndefinedMemberError(binaryExpr.getPosition(), lhs->getName(), expectedFunctionName));
}

void TypeChecker::visitCallExpr(CallExpr& callExpr) {
	EXPR_ACCEPT(callExpr.getFunction(), *this, TypeReprHelper gFunction);
	const auto& function = std::dynamic_pointer_cast<FunctionType>(*gFunction);
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
		const TypeReprHelper param = function->getParameters()[i];
		EXPR_ACCEPT(callExpr.getArguments()[i], *this, TypeReprHelper arg);
		if(!arg->canBeAssignedTo(*param))
			THROW_TYPING_ERROR(WrongTypeError(callExpr.getPosition(), param->getName(), arg->getName()));
	}

	EXPR_RETURN_FROM_VISIT(function->getReturnType());
}

void TypeChecker::visitFunction(FunctionExpr& functionExpr) {
	FunctionType* enclosingFunctionType = m_currentFunction;
	EXPR_ACCEPT(functionExpr.getReturnType(), *this, TypeReprHelper returnType);
	std::vector<Type::Repr> parameters;
	TypeContext::Ptr paramScope = runInNewContext(
			[&functionExpr, &parameters, this]() {
				for(const auto& param : functionExpr.getParameters()) {
					const auto& result = m_currentContext->getType(param.type);
					if(!result.has_value())
						THROW_TYPING_ERROR(UndefinedTypeError(functionExpr.getPosition(), param.type));
					if(TypeReprHelper type = result.value(); !type->isApplicableForDeclaration())
						THROW_TYPING_ERROR(UndefinedTypeError(functionExpr.getPosition(), type->getName()));
					parameters.push_back(result.value());
					if(!m_currentContext->declareVar(param.name.getValue().asString(), result.value(), true))
						THROW_TYPING_ERROR(AlreadyDefinedVariableError(functionExpr.getPosition(),
								param.name.getValue().asString()));
				}
			},
			m_currentContext);

	Type::Ptr functionType = Type::makePtr<FunctionType>(returnType.getRepr(), parameters);
	m_currentFunction = std::dynamic_pointer_cast<FunctionType>(functionType).get();

	runInNewContext([&functionExpr, this]() { functionExpr.getImplementation()->accept(*this); },
			m_currentContext,
			paramScope);
	if(TypeReprHelper currentFunctionReturnType = m_currentFunction->getReturnType();
			!m_doesCurrentFunctionReturn && currentFunctionReturnType->getName() != Value::NativeTypes::Nothing)
		THROW_TYPING_ERROR(WrongTypeError(functionExpr.getPosition(),
				currentFunctionReturnType->getName(),
				Value::NativeTypes::Nothing));

	m_doesCurrentFunctionReturn = false;
	m_currentFunction = enclosingFunctionType;
	EXPR_RETURN_FROM_VISIT(TypeProvider::the().encode(functionType));
}

void TypeChecker::visitGroupExpr(GroupExpr& groupExpr) {
	EXPR_ACCEPT(groupExpr.getExpr(), *this, TypeReprHelper type);
	EXPR_RETURN_FROM_VISIT(type);
}

void TypeChecker::visitInstantiationExpr(InstantiationExpr& instantiationExpr) {
	const auto& result = m_currentContext->getType(instantiationExpr.getName());
	if(!result.has_value())
		THROW_TYPING_ERROR(UndefinedTypeError(instantiationExpr.getPosition(), instantiationExpr.getName()));
	auto classType = std::dynamic_pointer_cast<ClassType>(TypeProvider::the().decode(result.value()));
	if(instantiationExpr.getArguments().size() != classType->getArity())
		THROW_TYPING_ERROR(ArityError(instantiationExpr.getPosition(),
				classType->getArity(),
				instantiationExpr.getArguments().size(),
				"Constructor of class " + instantiationExpr.getName()));
	for(unsigned long i = 0; i < classType->getArity(); ++i) {
		TypeReprHelper expected = classType->getConstructorParameter()[i];
		EXPR_ACCEPT(instantiationExpr.getArguments()[i], *this, TypeReprHelper provided);
		if(expected != provided)
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
		EXPR_ACCEPT(typeExpr.getReturnType(), *this, TypeReprHelper returnType);
		std::vector<Type::Repr> paramTypes;
		for(const auto& param : typeExpr.getParameterTypes()) {
			EXPR_ACCEPT(param, *this, TypeReprHelper paramType);
			paramTypes.push_back(paramType.getRepr());
		}
		EXPR_RETURN_FROM_VISIT(
				TypeProvider::the().encode(Type::makePtr<FunctionType>(returnType.getRepr(), paramTypes)));
	}
	const auto& result = m_currentContext->getType(typeExpr.getName());
	if(!result.has_value())
		THROW_TYPING_ERROR(UndefinedTypeError(typeExpr.getPosition(), typeExpr.getName()));
	EXPR_RETURN_FROM_VISIT(result.value());
}

void TypeChecker::visitUnaryExpr(UnaryExpr& unaryExpr) {
	EXPR_ACCEPT(unaryExpr.getRhs(), *this, TypeReprHelper rhs);

	const std::string expectedFunctionName = "operator" + unaryExpr.getOperator().getLexeme();
	if(const auto& res = rhs->knowsAbout(expectedFunctionName); res.has_value()) {
		Type::Ptr type = TypeProvider::the().decode(res->value);
		if(type->canBeCalledWith({}))
			EXPR_RETURN_FROM_VISIT(std::dynamic_pointer_cast<FunctionType>(type)->getReturnType());
	}
	THROW_TYPING_ERROR(UndefinedMemberError(unaryExpr.getPosition(), rhs->getName(), expectedFunctionName));
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

	TypeReprHelper expectedType{};
	if(declarationStmt.getType() == nullptr) {
		if(declarationStmt.getInitializer() == nullptr)
			THROW_TYPING_ERROR(TypingError(declarationStmt.getPosition(),
					"Variable " + name + " needs to be explicitly typed, as it does not get initialized immediately"));
		EXPR_ACCEPT(declarationStmt.getInitializer(), *this, expectedType);
	} else {
		EXPR_ACCEPT(declarationStmt.getType(), *this, expectedType);
		if(!expectedType->isApplicableForDeclaration())
			THROW_TYPING_ERROR(UndefinedTypeError(declarationStmt.getPosition(), expectedType->getName()));
		if(declarationStmt.getInitializer() != nullptr) {
			if(expectedType->isFunction())
				m_currentContext->declareVar(name, expectedType.getRepr(), declarationStmt.isMutable());
			EXPR_ACCEPT(declarationStmt.getInitializer(), *this, TypeReprHelper initType);
			if(expectedType->isFunction())
				m_currentContext->removeVar(name);
			if(!initType->canBeAssignedTo(*expectedType))
				THROW_TYPING_ERROR(
						WrongTypeError(declarationStmt.getPosition(), expectedType->getName(), initType->getName()));
		}
	}
	if(!m_currentContext->declareVar(name, expectedType.getRepr(), declarationStmt.isMutable())) {
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

	auto* klass = new ClassType(name);
	m_currentContext->declareType(name, TypeProvider::the().encode(ClassType::Ptr(klass)));

	TypeContext::Ptr paramScope = runInNewContext(
			[&classDeclarationStmt, &klass, this]() {
				for(const auto& param : classDeclarationStmt.getConstructorParameters()) {
					const auto& result = m_currentContext->getType(param.type);
					if(!result.has_value())
						THROW_TYPING_ERROR(UndefinedTypeError(classDeclarationStmt.getPosition(), param.type));
					if(TypeReprHelper resultType = result.value(); !resultType->isApplicableForDeclaration())
						THROW_TYPING_ERROR(
								UndefinedTypeError(classDeclarationStmt.getPosition(), resultType->getName()));
					klass->addConstructorParam(result.value());
					klass->addDeclaration(param.name.getValue().asString(),
							Variable<Type::Repr>(result.value(), param.isMutable));
					if(!m_currentContext->declareVar(param.name.getValue().asString(), result.value(), param.isMutable))
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

	for(const auto& [declName, decl] : declScope->getVariables())
		klass->addDeclaration(declName, decl);

	m_currentClassName = nullptr;
}
void TypeChecker::visitExpressionStmt(ExpressionStmt& expressionStmt) { expressionStmt.getExpr()->accept(*this); }

void TypeChecker::visitPrintStmt(PrintStmt& printStmt) { EXPR_ACCEPT(printStmt.getExpr(), *this, TypeReprHelper type); }

void TypeChecker::visitReturnStmt(ReturnStmt& returnStmt) {
	if(m_currentFunction == nullptr)
		THROW_TYPING_ERROR(InvalidReturnError(returnStmt.getPosition()));
	EXPR_ACCEPT(returnStmt.getExpr(), *this, TypeReprHelper returnedType);
	if(returnedType != m_currentFunction->getReturnType())
		THROW_TYPING_ERROR(WrongTypeError(returnStmt.getPosition(),
				TypeProvider::the().decode(m_currentFunction->getReturnType())->getName(),
				returnedType->getName()));
	m_doesCurrentFunctionReturn = true;
}
void TypeChecker::visitWhileStmt(WhileStmt& whileStmt) {
	EXPR_ACCEPT(whileStmt.getCondition(), *this, TypeReprHelper condition);
	whileStmt.getStatement()->accept(*this);
}
}