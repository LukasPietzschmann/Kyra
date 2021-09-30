#include "TypeChecker.hpp"

namespace LibSlg {
TypeChecker& TypeChecker::getInstance() {
	static TypeChecker instance;
	return instance;
}

TypeChecker::Result TypeChecker::check(const std::vector<Statement::Ptr>& statements, bool passThroughExceptions) {
	TypeChecker::Result result;
	for(const auto& statement : statements) {
		try {
			check(statement);
		} catch(TypingException& exception) {
			if(passThroughExceptions)
				throw TypingException(exception);
			result.insertError(exception.what());
		}
	}
	return result;
}

	return result;
}

void TypeChecker::check(const Statement::Ptr& statement) { statement->accept(*this); }

TypeChecker::Scope* TypeChecker::runInNewScope(
		const std::function<void()>& function, Scope* parent, Scope* valuesToCopy) {
	Scope* globalScopeCopy = m_currentScope;
	auto* newGlobalScope = new Scope(parent);
	if(valuesToCopy != nullptr) {
		for(const auto& var : valuesToCopy->getVariables())
			newGlobalScope->setVar(var.first, var.second);
		for(const auto& type : valuesToCopy->getTypes())
			newGlobalScope->setType(type.first, type.second);
	}
	m_currentScope = newGlobalScope;
	function();
	m_currentScope = globalScopeCopy;
	return newGlobalScope;
}

void TypeChecker::visitAccessExpr(AccessExpr& accessExpr) {
	EXPR_ACCEPT(accessExpr.getOwner(), *this, Type::Ptr ownerType);
	const std::string& name = accessExpr.getName().getValue().asString();
	const auto& result = ownerType->knowsAbout(name);
	if(!result.has_value())
		throw UndefinedMemberException(accessExpr.getPosition(), ownerType->getName(), name);
	EXPR_RETURN_FROM_VISIT(result->type);
}

void TypeChecker::visitAssignmentExpr(AssignmentExpr& assignmentExpr) {
	Type::Ptr assignedValueType;
	const std::string& name = assignmentExpr.getName().getValue().asString();

	if(assignmentExpr.getOwner() != nullptr) {
		EXPR_ACCEPT(assignmentExpr.getOwner(), *this, Type::Ptr ownerType);
		assignedValueType = ownerType->knowsAbout(name)->type;
	} else
		assignedValueType = m_currentScope->getVar(name)->type;

	if(assignedValueType == nullptr)
		throw UndefinedVariableException(assignmentExpr.getPosition(), name);
	if(!m_currentScope->getVar(name)->isMutable)
		throw AssignmentToConstException(assignmentExpr.getPosition(), name);
	EXPR_ACCEPT(assignmentExpr.getNewValue(), *this, Type::Ptr newValueType);
	if(*newValueType != assignedValueType)
		throw WrongTypeException(assignmentExpr.getPosition(), assignedValueType->getName(), newValueType->getName());
	EXPR_RETURN_FROM_VISIT(newValueType);
}

void TypeChecker::visitBinaryExpr(BinaryExpr& binaryExpr) {}

void TypeChecker::visitCallExpr(CallExpr& callExpr) {
	EXPR_ACCEPT(callExpr.getFunction(), *this, Type::Ptr gFunction);
	const auto& function = std::dynamic_pointer_cast<FunctionType>(gFunction);
	if(function == nullptr)
		throw WrongTypeException(callExpr.getPosition(), Value::NativeTypes::Function, gFunction->getName());
	if(function->getParameters().size() != callExpr.getArguments().size())
		throw ArityException(
				callExpr.getPosition(), function->getArity(), callExpr.getArguments().size(), function->getName());
	for(unsigned long i = 0; i < function->getParameters().size(); ++i) {
		const Type& param = *function->getParameters()[i];
		EXPR_ACCEPT(callExpr.getArguments()[i], *this, Type::Ptr arg);
		if(param != arg)
			throw WrongTypeException(callExpr.getPosition(), param.getName(), arg->getName());
	}

	EXPR_RETURN_FROM_VISIT(function->getReturnType());
}

void TypeChecker::visitFunction(FunctionExpr& functionExpr) {
	FunctionType* enclosingFunctionType = m_currentFunction;
	EXPR_ACCEPT(functionExpr.getReturnType(), *this, Type::Ptr returnType);
	std::vector<Type::Ptr> parameters;
	Scope* paramScope = runInNewScope(
			[&functionExpr, &parameters, this]() {
				for(const auto& param : functionExpr.getParameters()) {
					const auto& result = m_currentScope->getType(param.type);
					if(!result.has_value())
						throw UndefinedTypeException(functionExpr.getPosition(), param.type);
					parameters.push_back(result.value());
					if(!m_currentScope->setVar(param.name.getValue().asString(), result.value(), true))
						throw AlreadyDefinedVariableException(
								functionExpr.getPosition(), param.name.getValue().asString());
				}
			},
			m_currentScope);

	Type::Ptr functionType = Type::makePtr<FunctionType>(returnType, parameters);
	m_currentFunction = std::dynamic_pointer_cast<FunctionType>(functionType).get();

	runInNewScope(
			[&functionExpr, this]() { functionExpr.getImplementation()->accept(*this); }, m_currentScope, paramScope);

	if(!m_doesCurrentFunctionReturn && m_currentFunction->getReturnType()->getName() != Value::NativeTypes::Nothing)
		throw WrongTypeException(
				functionExpr.getPosition(), m_currentFunction->getReturnType()->getName(), Value::NativeTypes::Nothing);

	m_doesCurrentFunctionReturn = false;
	m_currentFunction = enclosingFunctionType;
	EXPR_RETURN_FROM_VISIT(functionType);
}

void TypeChecker::visitGroupExpr(GroupExpr& groupExpr) {
	EXPR_ACCEPT(groupExpr.getExpr(), *this, Type::Ptr type);
	EXPR_RETURN_FROM_VISIT(type);
}

void TypeChecker::visitInstantiationExpr(InstantiationExpr& instantiationExpr) {
	const auto& result = m_currentScope->getType(instantiationExpr.getName());
	if(!result.has_value())
		throw UndefinedTypeException(instantiationExpr.getPosition(), instantiationExpr.getName());
	auto classType = std::dynamic_pointer_cast<ClassType>(result.value());
	if(instantiationExpr.getArguments().size() != classType->getArity())
		throw ArityException(instantiationExpr.getPosition(), classType->getArity(),
				instantiationExpr.getArguments().size(), "Constructor of class " + instantiationExpr.getName());
	for(unsigned long i = 0; i < classType->getArity(); ++i) {
		const Type::Ptr& expected = classType->getConstructorParameter()[i];
		EXPR_ACCEPT(instantiationExpr.getArguments()[i], *this, Type::Ptr provided);
		if(*expected != provided)
			throw WrongTypeException(instantiationExpr.getPosition(), expected->getName(), provided->getName());
	}
	EXPR_RETURN_FROM_VISIT(result.value());
}

void TypeChecker::visitLiteral(LiteralExpr& literalExpr) {
	EXPR_RETURN_FROM_VISIT(m_currentScope->getType(literalExpr.getValue()->getType()).value());
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
	const auto& result = m_currentScope->getType(typeExpr.getName());
	if(!result.has_value())
		throw UndefinedTypeException(typeExpr.getPosition(), typeExpr.getName());
	EXPR_RETURN_FROM_VISIT(result.value());
}

void TypeChecker::visitUnaryExpr(UnaryExpr& unaryExpr) {
	// TODO check for function in type. This allows for overriding those functions
	const TokenType& oper = unaryExpr.getOperator().getType();
	EXPR_ACCEPT(unaryExpr.getRhs(), *this, Type::Ptr rhs);
	switch(oper) {
		case TokenType::BANG: EXPR_RETURN_FROM_VISIT(m_currentScope->getType(Value::NativeTypes::Bool).value());
		case TokenType::MINUS:
			if(rhs->getName() == Value::NativeTypes::Number)
				EXPR_RETURN_FROM_VISIT(NativeTypes::makeNativeType(Value::NativeTypes::Number));
			else
				throw WrongTypeException(unaryExpr.getPosition(), Value::NativeTypes::Number, rhs->getName());
		default: assert(false);
	}
}

void TypeChecker::visitVariable(VariableExpr& variableExpr) {
	const std::string& name = variableExpr.getName().getValue().asString();
	const auto& result = m_currentScope->getVar(name);
	if(!result.has_value()) {
		if(m_currentClassName != nullptr)
			throw UndefinedMemberException(variableExpr.getPosition(), std::string(m_currentClassName), name);
		else
			throw UndefinedVariableException(variableExpr.getPosition(), name);
	}
	EXPR_RETURN_FROM_VISIT(result->type);
}

void TypeChecker::visitBlockStmt(BlockStmt& blockStmt) {
	Scope* scope = runInNewScope(
			[&]() {
				for(const auto& statement : blockStmt.getStatements())
					statement->accept(*this);
			},
			m_currentScope);
	STMT_RETURN_FROM_VISIT(scope);
}

void TypeChecker::visitDeclarationStmt(DeclarationStmt& declarationStmt) {
	const std::string& name = declarationStmt.getIdentifier().getValue().asString();
	EXPR_ACCEPT(declarationStmt.getType(), *this, Type::Ptr expectedType);
	if(declarationStmt.getInitializer() != nullptr) {
		EXPR_ACCEPT(declarationStmt.getInitializer(), *this, Type::Ptr initType);
		if(*expectedType != initType)
			throw WrongTypeException(declarationStmt.getPosition(), expectedType->getName(), initType->getName());
	}
	Type::Ptr instanceType = expectedType->duplicate();
	if(!m_currentScope->setVar(name, instanceType, declarationStmt.isMutable())) {
		if(m_currentClassName != nullptr)
			throw AlreadyDefinedMemberException(declarationStmt.getPosition(), std::string(m_currentClassName), name);
		else
			throw AlreadyDefinedVariableException(declarationStmt.getPosition(), name);
	}
	STMT_RETURN_FROM_VISIT(m_currentScope);
}

void TypeChecker::visitClassDeclarationStmt(ClassDeclarationStmt& classDeclarationStmt) {
	m_currentClassName = classDeclarationStmt.getIdentifier().getValue().asString().data();
	const std::string& name = classDeclarationStmt.getIdentifier().getValue().asString();
	if(m_currentScope->getType(name).has_value())
		throw AlreadyDefinedTypeException(classDeclarationStmt.getPosition(), name);

	Scope* declScope = runInNewScope(
			[&classDeclarationStmt, this]() {
				for(const auto& decl : classDeclarationStmt.getDeclarations())
					decl->accept(*this);  // TODO print throwt hier drin, deswegen wird die klasse nicht
										  // deklariert.nicht throwen sindern einer liste anfügen
			},
			m_currentScope);

	std::vector<Type::Ptr> constructorParams;
	for(const auto& param : classDeclarationStmt.getConstructorParameters()) {
		const auto& result = m_currentScope->getType(param.type);
		if(!result.has_value())
			assert(false);	// TODO throw error
		Type::Ptr type = result.value()->duplicate();
		constructorParams.push_back(type);
		if(!declScope->setVar(param.name.getValue().asString(), type, param.isMutable))
			throw AlreadyDefinedVariableException(classDeclarationStmt.getPosition(), param.name.getValue().asString());
	}
	m_currentScope->setType(name, Type::makePtr<ClassType>(name, declScope->getVariables(), constructorParams));
	m_currentClassName = nullptr;
	STMT_RETURN_FROM_VISIT(m_currentScope);
}

void TypeChecker::visitExpressionStmt(ExpressionStmt& expressionStmt) {
	expressionStmt.getExpr()->accept(*this);
	STMT_RETURN_FROM_VISIT(m_currentScope);
}

void TypeChecker::visitPrintStmt(PrintStmt& printStmt) {
	EXPR_ACCEPT(printStmt.getExpr(), *this, Type::Ptr type);
	STMT_RETURN_FROM_VISIT(m_currentScope);
}

void TypeChecker::visitReturnStmt(ReturnStmt& returnStmt) {
	if(m_currentFunction == nullptr)
		throw InvalidReturnException(returnStmt.getPosition());
	EXPR_ACCEPT(returnStmt.getExpr(), *this, Type::Ptr returnedType);
	if(*returnedType != m_currentFunction->getReturnType())
		throw WrongTypeException(
				returnStmt.getPosition(), m_currentFunction->getReturnType()->getName(), returnedType->getName());
	m_doesCurrentFunctionReturn = true;
	STMT_RETURN_FROM_VISIT(m_currentScope);
}
}