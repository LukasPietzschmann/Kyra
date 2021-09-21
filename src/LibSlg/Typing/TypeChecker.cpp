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
				throw exception;
			result.insertError(exception.what());
		}
	}
	return result;
}

TypeChecker::Scope::Ptr TypeChecker::check(const Statement::Ptr& statement) {
	STMT_ACCEPT(statement, *this, Scope::Ptr result);
	return result;
}

TypeChecker::Scope::Ptr TypeChecker::runInNewScope(
		const std::function<void()>& function, const Scope::Ptr& parent, const Scope* valuesToCopy) {
	Scope::Ptr globalScopeCopy = std::make_shared<Scope>(m_currentScope);
	Scope::Ptr newGlobalScope(parent);
	if(valuesToCopy != nullptr) {
		for(const auto& var : valuesToCopy->getVariables())
			newGlobalScope->setVar(var.first, var.second);
		for(const auto& type : valuesToCopy->getTypes())
			newGlobalScope->setType(type.first, type.second);
	}
	m_currentScope = newGlobalScope;
	function();
	m_currentScope = std::move(globalScopeCopy);
	return newGlobalScope;
}

void TypeChecker::visitAccessExpr(AccessExpr& accessExpr) {
	EXPR_ACCEPT(accessExpr.getOwner(), *this, Type::Ptr ownerType);
	const std::string& name = accessExpr.getName().getValue().asString();
	Type::Ptr accessedValueType = ownerType->knowsAbout(name);
	if(accessedValueType == nullptr)
		throw UndefinedMemberException(name, ownerType->getName());
	EXPR_RETURN_FROM_VISIT(accessedValueType);
}

void TypeChecker::visitAssignmentExpr(AssignmentExpr& assignmentExpr) {
	Type::Ptr assignedValueType;
	const std::string& name = assignmentExpr.getName().getValue().asString();

	if(assignmentExpr.getOwner() != nullptr) {
		EXPR_ACCEPT(assignmentExpr.getOwner(), *this, Type::Ptr ownerType);
		assignedValueType = ownerType->knowsAbout(name);
	} else
		assignedValueType = m_currentScope->getVar(name);

	if(assignedValueType == nullptr)
		throw UndefinedVariableException(name);
	if(!assignedValueType->isMutable())
		throw AssignmentToConstException(name);
	EXPR_ACCEPT(assignmentExpr.getNewValue(), *this, Type::Ptr newValueType);
	if(*newValueType != assignedValueType)
		throw WrongTypeException(assignedValueType->getName(), newValueType->getName());
	EXPR_RETURN_FROM_VISIT(newValueType);
}

void TypeChecker::visitBinaryExpr(BinaryExpr& binaryExpr) {}

void TypeChecker::visitCallExpr(CallExpr& callExpr) {
	EXPR_ACCEPT(callExpr.getFunction(), *this, Type::Ptr gFunction);
	const auto& function = std::dynamic_pointer_cast<FunctionType>(gFunction);
	if(function == nullptr)
		throw WrongTypeException(Value::NativeTypes::Function, gFunction->getName());
	if(function->getParameters().size() != callExpr.getArguments().size())
		throw ArityException(function->getArity(), callExpr.getArguments().size(), function->getName());
	for(unsigned long i = 0; i < function->getParameters().size(); ++i) {
		const Type& param = *function->getParameters()[i];
		EXPR_ACCEPT(callExpr.getArguments()[i], *this, Type::Ptr arg);
		if(param != arg)
			throw WrongTypeException(param.getName(), arg->getName());
	}

	EXPR_RETURN_FROM_VISIT(function->getReturnType());
}

void TypeChecker::visitFunction(FunctionExpr& functionExpr) {
	FunctionType* enclosingFunctionType = m_currentFunction;
	EXPR_ACCEPT(functionExpr.getReturnType(), *this, Type::Ptr returnType);
	std::vector<Type::Ptr> parameters;
	Scope::Ptr paramScope = runInNewScope(
			[&functionExpr, &parameters, this]() {
				for(const auto& param : functionExpr.getParameters()) {
					Type::Ptr paramType = m_currentScope->getType(param.type);
					if(paramType == nullptr)
						throw UndefinedTypeException(param.type);
					parameters.push_back(paramType);
					if(!m_currentScope->setVar(param.name.getValue().asString(), paramType))
						throw AlreadyDefinedVariableException(param.name.getValue().asString());
				}
			},
			m_currentScope);

	Type::Ptr functionType = Type::makePtr<FunctionType>(false, returnType, parameters);
	m_currentFunction = std::dynamic_pointer_cast<FunctionType>(functionType).get();

	runInNewScope([&functionExpr, this]() { functionExpr.getImplementation()->accept(*this); }, m_currentScope,
			paramScope.get());

	if(!m_doesCurrentFunctionReturn && m_currentFunction->getReturnType()->getName() != Value::NativeTypes::Nothing)
		throw WrongTypeException(m_currentFunction->getReturnType()->getName(), Value::NativeTypes::Nothing);

	m_doesCurrentFunctionReturn = false;
	m_currentFunction = enclosingFunctionType;
	EXPR_RETURN_FROM_VISIT(functionType);
}

void TypeChecker::visitGroupExpr(GroupExpr& groupExpr) {
	EXPR_ACCEPT(groupExpr.getExpr(), *this, Type::Ptr type);
	EXPR_RETURN_FROM_VISIT(type);
}

void TypeChecker::visitInstantiationExpr(InstantiationExpr& instantiationExpr) {
	Type::Ptr type = m_currentScope->getType(instantiationExpr.getName());
	if(type == nullptr)
		throw UndefinedTypeException(instantiationExpr.getName());
	auto classType = std::dynamic_pointer_cast<ClassType>(type);
	if(instantiationExpr.getArguments().size() != classType->getArity())
		throw ArityException(classType->getArity(), instantiationExpr.getArguments().size(),
				"Constructor of class " + instantiationExpr.getName());
	for(unsigned long i = 0; i < classType->getArity(); ++i) {
		const Type::Ptr& expected = classType->getConstructorParameter()[i];
		EXPR_ACCEPT(instantiationExpr.getArguments()[i], *this, Type::Ptr provided);
		if(*expected != provided)
			throw WrongTypeException(expected->getName(), provided->getName());
	}
	EXPR_RETURN_FROM_VISIT(type);
}

void TypeChecker::visitLiteral(LiteralExpr& literalExpr) {
	EXPR_RETURN_FROM_VISIT(m_currentScope->getType(literalExpr.getValue()->getType()));
}

void TypeChecker::visitTypeExpr(TypeExpr& typeExpr) {
	if(typeExpr.isFunction()) {
		EXPR_ACCEPT(typeExpr.getReturnType(), *this, Type::Ptr returnType);
		std::vector<Type::Ptr> paramTypes;
		for(const auto& param : typeExpr.getParameterTypes()) {
			EXPR_ACCEPT(param, *this, Type::Ptr paramType);
			paramTypes.push_back(paramType);
		}
		EXPR_RETURN_FROM_VISIT(Type::makePtr<FunctionType>(false, returnType, paramTypes));
	}
	Type::Ptr type = m_currentScope->getType(typeExpr.getName());
	if(type == nullptr)
		throw UndefinedTypeException(typeExpr.getName());
	EXPR_RETURN_FROM_VISIT(type);
}

void TypeChecker::visitUnaryExpr(UnaryExpr& unaryExpr) {
	// TODO check for function in type. This allows for overriding those functions
	const TokenType& oper = unaryExpr.getOperator().getType();
	EXPR_ACCEPT(unaryExpr.getRhs(), *this, Type::Ptr rhs);
	switch(oper) {
		case TokenType::BANG: EXPR_RETURN_FROM_VISIT(m_currentScope->getType(Value::NativeTypes::Bool));
		case TokenType::MINUS:
			if(rhs->getName() == Value::NativeTypes::Number)
				EXPR_RETURN_FROM_VISIT(NativeTypes::make(Value::NativeTypes::Number));
			else
				throw WrongTypeException(Value::NativeTypes::Number, rhs->getName());
		default: assert(false);
	}
}

void TypeChecker::visitVariable(VariableExpr& variableExpr) {
	const std::string& name = variableExpr.getName().getValue().asString();
	Type::Ptr type = m_currentScope->getVar(name);
	if(type == nullptr) {
		if(m_currentClassName != nullptr)
			throw UndefinedMemberException(std::string(m_currentClassName), name);
		else
			throw UndefinedVariableException(name);
	}
	EXPR_RETURN_FROM_VISIT(type);
}

void TypeChecker::visitBlockStmt(BlockStmt& blockStmt) {
	Scope::Ptr scope = runInNewScope(
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
			throw WrongTypeException(expectedType->getName(), initType->getName());
	}
	Type::Ptr instanceType = expectedType->duplicate();
	instanceType->setMutable(declarationStmt.isMutable());
	if(!m_currentScope->setVar(name, instanceType)) {
		if(m_currentClassName != nullptr)
			throw AlreadyDefinedMemberException(std::string(m_currentClassName), name);
		else
			throw AlreadyDefinedVariableException(name);
	}
	STMT_RETURN_FROM_VISIT(m_currentScope);
}

void TypeChecker::visitClassDeclarationStmt(ClassDeclarationStmt& classDeclarationStmt) {
	m_currentClassName = classDeclarationStmt.getIdentifier().getValue().asString().data();
	const std::string& name = classDeclarationStmt.getIdentifier().getValue().asString();
	if(m_currentScope->getType(name))
		throw AlreadyDefinedTypeException(name);

	Scope::Ptr declScope = runInNewScope(
			[&classDeclarationStmt, this]() {
				for(const auto& decl : classDeclarationStmt.getDeclarations())
					decl->accept(*this);
			},
			m_currentScope);

	std::vector<Type::Ptr> constructorParams;
	for(const auto& param : classDeclarationStmt.getConstructorParameters()) {
		Type::Ptr type = m_currentScope->getType(param.type)->duplicate();
		type->setMutable(param.isMutable);
		constructorParams.push_back(type);
		if(!declScope->setVar(param.name.getValue().asString(), type))
			throw AlreadyDefinedVariableException(param.name.getValue().asString());
	}
	m_currentScope->setType(name, Type::makePtr<ClassType>(name, false, declScope->getVariables(), constructorParams));
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
		throw InvalidReturnException();
	EXPR_ACCEPT(returnStmt.getExpr(), *this, Type::Ptr returnedType);
	if(*returnedType != m_currentFunction->getReturnType())
		throw WrongTypeException(m_currentFunction->getReturnType()->getName(), returnedType->getName());
	m_doesCurrentFunctionReturn = true;
	STMT_RETURN_FROM_VISIT(m_currentScope);
}
}