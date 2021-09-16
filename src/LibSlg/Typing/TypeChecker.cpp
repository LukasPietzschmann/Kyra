#include "TypeChecker.hpp"

namespace LibSlg {
TypeChecker& TypeChecker::getInstance() {
	static TypeChecker instance;
	return instance;
}

void TypeChecker::check(const std::vector<Statement::Ptr>& statements) {
	for(const auto& statement : statements)
		check(statement);
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
		throw TypingException("Type " + ownerType->getName() + " does not contain a variable named " + name);
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
		throw TypingException("No variable named " + name);
	if(!assignedValueType->isMutable())
		throw TypingException("Asignee " + name + " is not mutable");
	EXPR_ACCEPT(assignmentExpr.getNewValue(), *this, Type::Ptr newValueType);
	if(*newValueType != assignedValueType)
		throw WrongTypeException(assignedValueType->getName(), newValueType->getName());
	EXPR_RETURN_FROM_VISIT(newValueType);
}

void TypeChecker::visitBinaryExpr(BinaryExpr& binaryExpr) {}

void TypeChecker::visitCallExpr(CallExpr& callExpr) {}

void TypeChecker::visitFunction(FunctionExpr& functionExpr) {
	EXPR_ACCEPT(functionExpr.getReturnType(), *this, Type::Ptr returnType);
	std::vector<Type::Ptr> parameters;
	Scope::Ptr paramScope = runInNewScope(
			[&functionExpr, &parameters, this]() {
				for(const auto& param : functionExpr.getParameters()) {
					Type::Ptr paramType = m_currentScope->getType(param.type);
					if(paramType == nullptr)
						throw TypingException("Unknown type " + param.type);
					parameters.push_back(paramType);
					if(!m_currentScope->setVar(param.name.getValue().asString(), paramType))
						throw TypingException("Dat gibts schon " + param.name.getValue().asString());
				}
			},
			m_currentScope);

	Type::Ptr functionType = Type::makePtr<FunctionType>(false, returnType, parameters);
	m_currentFunction = std::dynamic_pointer_cast<FunctionType>(functionType).get();

	runInNewScope([&functionExpr, this]() { functionExpr.getImplementation()->accept(*this); }, m_currentScope,
			paramScope.get());

	m_currentFunction = nullptr;
	EXPR_RETURN_FROM_VISIT(functionType);
}

void TypeChecker::visitGroupExpr(GroupExpr& groupExpr) {
	EXPR_ACCEPT(groupExpr.getExpr(), *this, Type::Ptr type);
	EXPR_RETURN_FROM_VISIT(type);
}

void TypeChecker::visitInstantiationExpr(InstantiationExpr& instantiationExpr) {
	Type::Ptr type = m_currentScope->getType(instantiationExpr.getName());
	if(type == nullptr)
		throw TypingException("Class " + instantiationExpr.getName() + " not known");
	auto classType = std::dynamic_pointer_cast<ClassType>(type);
	if(instantiationExpr.getArguments().size() != classType->getArity())
		throw TypingException("Constructor of class " + instantiationExpr.getName() + " has to be instantiated with " +
							  std::to_string(classType->getArity()) + " argument, but you provided " +
							  std::to_string(instantiationExpr.getArguments().size()));
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
		throw TypingException("Unknown type " + typeExpr.getName());
	EXPR_RETURN_FROM_VISIT(type);
}

void TypeChecker::visitUnaryExpr(UnaryExpr& unaryExpr) {}

void TypeChecker::visitVariable(VariableExpr& variableExpr) {
	const std::string& name = variableExpr.getName().getValue().asString();
	Type::Ptr type = m_currentScope->getVar(name);
	if(type == nullptr)
		throw TypingException("Variable " + name + " is not declared");
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
	// FIXME this does not work with custom classes
	EXPR_ACCEPT(declarationStmt.getType(), *this, Type::Ptr expectedType);
	if(declarationStmt.getInitializer() != nullptr) {
		EXPR_ACCEPT(declarationStmt.getInitializer(), *this, Type::Ptr initType);
		if(*expectedType != initType)
			throw WrongTypeException(expectedType->getName(), initType->getName());
	}
	Type::Ptr instanceType = expectedType->duplicate();
	instanceType->setMutable(declarationStmt.isMutable());
	if(!m_currentScope->setVar(name, instanceType))
		throw TypingException("Variable " + name + " already declared");
	STMT_RETURN_FROM_VISIT(m_currentScope);
}

void TypeChecker::visitClassDeclarationStmt(ClassDeclarationStmt& classDeclarationStmt) {
	const std::string& name = classDeclarationStmt.getIdentifier().getValue().asString();
	if(m_currentScope->getType(name))
		throw TypingException("Class " + name + " already declared");

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
			throw TypingException("Mich gibts schon");
	}
	m_currentScope->setType(name, Type::makePtr<ClassType>(name, false, declScope->getVariables(), constructorParams));
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
		throw TypingException("You can only return from a function context");
	EXPR_ACCEPT(returnStmt.getExpr(), *this, Type::Ptr returnedType);
	if(*returnedType != m_currentFunction->getReturnType())
		throw WrongTypeException(m_currentFunction->getReturnType()->getName(), returnedType->getName());
	STMT_RETURN_FROM_VISIT(m_currentScope);
}
}