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

TypeChecker::Scope TypeChecker::check(const Statement::Ptr& statement) {
	STMT_ACCEPT(statement, *this, Scope result);
	return result;
}

TypeChecker::Scope TypeChecker::runInNewScope(const std::function<void()>& function) {
	Scope globalScopeCopy = m_currentScope;
	Scope newGlobalScope;
	m_currentScope = newGlobalScope;
	function();
	newGlobalScope = m_currentScope;
	m_currentScope = globalScopeCopy;
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
	const std::string& name = assignmentExpr.getName().getValue().asString();
	if(assignmentExpr.getOwner() == nullptr) {
		auto it = m_currentScope.variables.find(name);
		if(it == m_currentScope.variables.end())
			throw TypingException("Variable " + name + " is not declared");
		if(!it->second->isMutable())
			throw TypingException("Asignee " + name + " is not mutable");
		EXPR_ACCEPT(assignmentExpr.getNewValue(), *this, Type::Ptr type);
		EXPR_RETURN_FROM_VISIT(type);
	}
	EXPR_ACCEPT(assignmentExpr.getOwner(), *this, Type::Ptr ownerType);
	Type::Ptr assignedValueType = ownerType->knowsAbout(name);
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

void TypeChecker::visitFunction(FunctionExpr& functionExpr) {}

void TypeChecker::visitGroupExpr(GroupExpr& groupExpr) {
	EXPR_ACCEPT(groupExpr.getExpr(), *this, Type::Ptr type);
	EXPR_RETURN_FROM_VISIT(type);
}

void TypeChecker::visitInstantiationExpr(InstantiationExpr& instantiationExpr) {
	auto it = m_currentScope.types.find(instantiationExpr.getName());
	if(it == m_currentScope.types.end())
		throw TypingException("Class " + instantiationExpr.getName() + " not known");
	auto classType = std::dynamic_pointer_cast<ClassType>(it->second);
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
	EXPR_RETURN_FROM_VISIT(it->second);
}

void TypeChecker::visitLiteral(LiteralExpr& literalExpr) {
	EXPR_RETURN_FROM_VISIT(m_currentScope.types.at(literalExpr.getValue()->getType()));
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
	const auto& it = m_currentScope.types.find(typeExpr.getName());
	if(it == m_currentScope.types.end())
		throw TypingException("Unknown type " + typeExpr.getName());
	EXPR_RETURN_FROM_VISIT(it->second);	 // TODO implement
}

void TypeChecker::visitUnaryExpr(UnaryExpr& unaryExpr) {}

void TypeChecker::visitVariable(VariableExpr& variableExpr) {
	const std::string& name = variableExpr.getName().getValue().asString();
	if(!m_currentScope.variables.contains(name))
		throw TypingException("Variable " + name + " is not declared");
	EXPR_RETURN_FROM_VISIT(m_currentScope.variables.at(name));
}

void TypeChecker::visitBlockStmt(BlockStmt& blockStmt) {
	Scope scope = runInNewScope([&]() {
		for(const auto& statement : blockStmt.getStatements())
			statement->accept(*this);
	});
	STMT_RETURN_FROM_VISIT(scope);
}

void TypeChecker::visitDeclarationStmt(DeclarationStmt& declarationStmt) {
	const std::string& name = declarationStmt.getIdentifier().getValue().asString();
	if(m_currentScope.variables.contains(name))
		throw TypingException("Variable " + name + " already declared");
	// FIXME this does not work with custom classes
	EXPR_ACCEPT(declarationStmt.getType(), *this, Type::Ptr expectedType);
	if(declarationStmt.getInitializer() != nullptr) {
		EXPR_ACCEPT(declarationStmt.getInitializer(), *this, Type::Ptr initType);
		if(*expectedType != initType)
			throw WrongTypeException(expectedType->getName(), initType->getName());
	}
	Type::Ptr instanceType = expectedType->duplicate();
	instanceType->setMutable(declarationStmt.isMutable());
	m_currentScope.variables.emplace(name, instanceType);
	STMT_RETURN_FROM_VISIT(m_currentScope);
}

void TypeChecker::visitClassDeclarationStmt(ClassDeclarationStmt& classDeclarationStmt) {
	const std::string& name = classDeclarationStmt.getIdentifier().getValue().asString();
	if(m_currentScope.types.contains(name))
		throw TypingException("Class " + name + " already declared");

	Scope declScope = runInNewScope([&classDeclarationStmt, this]() {
		for(const auto& decl : classDeclarationStmt.getDeclarations())
			decl->accept(*this);
	});

	std::vector<Type::Ptr> constructorParams;
	for(const auto& param : classDeclarationStmt.getConstructorParameters()) {
		Type::Ptr type = m_currentScope.types.at(param.type)->duplicate();
		type->setMutable(param.isMutable);
		constructorParams.push_back(type);
		declScope.variables.emplace(param.name.getValue().asString(), type);
	}
	Type::Ptr klass = Type::makePtr<ClassType>(name, false, declScope.variables, constructorParams);
	m_currentScope.types.emplace(name, klass);
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