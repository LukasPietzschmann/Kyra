#include "Interpreter.hpp"

#include "Values/Bool.hpp"

namespace LibSlg {
Interpreter& Interpreter::getInstance() {
	static Interpreter instance;
	return instance;
}

void Interpreter::execute(const std::string& code, bool verboseLogging, bool passThroughExceptions) {
	try {
		auto tokens = Lexer(code).scanTokens();
		auto statements = Parser(tokens).parse();
		if(verboseLogging) {
			AstLogger logger;
			for(const auto& token : tokens)
				std::cout << token << std::endl;
			for(const auto& statement : statements)
				logger.logStatement(statement);
		}
		try {
			TypeChecker::getInstance().check(statements);
		} catch(TypingException& exception) {
			if(passThroughExceptions)
				throw TypingException(exception);
			std::cout << "[Typing Error] " << exception.what() << std::endl;
			return;
		}
		for(const auto& statement : statements) {
			try {
				statement->accept(*this);
			} catch(RuntimeException& exception) {
				if(passThroughExceptions)
					throw RuntimeException(exception);
				std::cout << "[Runtime Error] " << exception.what() << std::endl;
			}
		}
	} catch(ParserException& exception) {
		if(passThroughExceptions)
			throw ParserException(exception);
		std::cout << "[Parser Error] " << exception.what() << std::endl;
	} catch(LexerException& exception) {
		if(passThroughExceptions)
			throw LexerException(exception);
		std::cout << "[Lexer Error] " << exception.what() << std::endl;
	}
}

bool Interpreter::isIncompleteStatement(const std::string& code) {
	try {
		Parser parser(Lexer(code).scanTokens());
		parser.parse();
	} catch(ParserException& exception) { return exception.isUnfinished(); } catch(...) {
		return false;
	}
	return false;
}

void Interpreter::visitAccessExpr(AccessExpr& accessExpr) {
	EXPR_ACCEPT(accessExpr.getOwner(), *this, Value::Ptr owner);
	auto klass = Value::as<Klass>(owner);
	if(klass == nullptr)
		throw RuntimeException("Variables can only be accessed on classes");
	if(!klass->knowsIdentifier(accessExpr.getName().getValue().asString()))
		throw RuntimeException("Class " + owner->getType() + " does not contain a variable named " +
							   accessExpr.getName().getValue().asString());
	else
		EXPR_RETURN_FROM_VISIT(klass->getInstanceContext()->getVar(accessExpr.getName().getValue().asString()).value);
}

void Interpreter::visitAssignmentExpr(AssignmentExpr& assignmentExpr) {
	Context::Ptr context = m_currentContext;
	if(assignmentExpr.getOwner() != nullptr) {
		EXPR_ACCEPT(assignmentExpr.getOwner(), *this, Value::Ptr owner);
		auto klass = Value::as<Klass>(owner);
		if(klass == nullptr)
			throw RuntimeException(assignmentExpr.getName().getValue().asString() + " is not an instance of a class");
		context = klass->getInstanceContext();
	}

	const Context::ContextValue& cValue = context->getVar(assignmentExpr.getName().getValue().asString());
	if(!cValue.isMutable)
		throw RuntimeException("Variable " + assignmentExpr.getName().getValue().asString() + " can't be rebound");
	EXPR_ACCEPT(assignmentExpr.getNewValue(), *this, Value::Ptr newValue);
	if(!newValue->hasCorrectTypeForAssignment(cValue.type))
		throw ParserException("Given type " + newValue->getType() + " does not match expected type " + cValue.type);
	context->mutate(assignmentExpr.getName().getValue().asString(), newValue);
	EXPR_RETURN_FROM_VISIT(newValue);
}

void Interpreter::visitBinaryExpr(BinaryExpr& binaryExpr) {
	EXPR_ACCEPT(binaryExpr.getLhs(), *this, Value::Ptr lhs);
	EXPR_ACCEPT(binaryExpr.getRhs(), *this, Value::Ptr rhs);
	switch(binaryExpr.getOperator().getType()) {
		case TokenType::EQUAL_EQUAL: EXPR_RETURN_FROM_VISIT(Value::makePtr<Bool>(*lhs == rhs));
		case TokenType::BANG_EQUAL: EXPR_RETURN_FROM_VISIT(Value::makePtr<Bool>(*lhs != rhs));
		case TokenType::GREATER: EXPR_RETURN_FROM_VISIT(Value::makePtr<Bool>(*lhs > rhs));
		case TokenType::GREATER_EQUAL: EXPR_RETURN_FROM_VISIT(Value::makePtr<Bool>(*lhs >= rhs));
		case TokenType::LESS: EXPR_RETURN_FROM_VISIT(Value::makePtr<Bool>(*lhs < rhs));
		case TokenType::LESS_EQUAL: EXPR_RETURN_FROM_VISIT(Value::makePtr<Bool>(*lhs <= rhs));
		case TokenType::MINUS:
			if(lhs->getType() != Value::NativeTypes::Number || rhs->getType() != Value::NativeTypes::Number)
				throw RuntimeException("The - Operator can only operate on two numbers");
			EXPR_RETURN_FROM_VISIT(*lhs - rhs);
		case TokenType::PLUS:
			if(!(lhs->getType() == Value::NativeTypes::Number && rhs->getType() == Value::NativeTypes::Number) &&
					!(lhs->getType() == Value::NativeTypes::String))
				throw RuntimeException("The + Operator can only operate on a string and two numbers");
			EXPR_RETURN_FROM_VISIT(*lhs + rhs);
		case TokenType::STAR:
			if((lhs->getType() != Value::NativeTypes::Number && lhs->getType() != Value::NativeTypes::String) ||
					rhs->getType() != Value::NativeTypes::Number)
				throw RuntimeException("The * Operator can only operate on two numbers or a string and a number");
			EXPR_RETURN_FROM_VISIT(*lhs * rhs);
		case TokenType::SLASH:
			if(lhs->getType() != Value::NativeTypes::Number || rhs->getType() != Value::NativeTypes::Number)
				throw RuntimeException("The / Operator can only operate on two numbers");
			EXPR_RETURN_FROM_VISIT(*lhs / rhs);
		default: assert(false);
	}
}

void Interpreter::visitCallExpr(CallExpr& callExpr) {
	EXPR_ACCEPT(callExpr.getFunction(), *this, Value::Ptr fun);
	unsigned int arity = Value::as<Function>(fun)->getArity();

	if(arity != callExpr.getArguments().size())
		throw RuntimeException("The Function needs to be called with " + std::to_string(arity) +
							   " arguments. You provided " + std::to_string(callExpr.getArguments().size()));

	std::vector<Value::Ptr> arguments;
	for(const auto& arg : callExpr.getArguments()) {
		EXPR_ACCEPT(arg, *this, Value::Ptr res);
		arguments.push_back(res);
	}

	Value::Ptr res = Value::as<Function>(fun)->exec(arguments);
	EXPR_RETURN_FROM_VISIT(res);
}

void Interpreter::visitFunction(FunctionExpr& functionExpr) {
	EXPR_RETURN_FROM_VISIT(Value::makePtr<Function>(functionExpr, m_currentContext));
}

void Interpreter::visitGroupExpr(GroupExpr& groupExpr) { return groupExpr.getExpr()->accept(*this); }

void Interpreter::visitInstantiationExpr(InstantiationExpr& instantiationExpr) {
	Value::Ptr klass = Value::makePtr<Klass>(m_currentContext->getCustomType(instantiationExpr.getName()));
	if(instantiationExpr.getArguments().size() != Value::as<Klass>(klass)->getArity())
		throw RuntimeException("The Class needs to be constructed with " +
							   std::to_string(Value::as<Klass>(klass)->getArity()) + " arguments. You provided " +
							   std::to_string(instantiationExpr.getArguments().size()));
	std::vector<Value::Ptr> values;
	for(const auto& argument : instantiationExpr.getArguments()) {
		EXPR_ACCEPT(argument, *this, Value::Ptr res);
		values.emplace_back(res);
	}
	Value::as<Klass>(klass)->instantiate(values);
	EXPR_RETURN_FROM_VISIT(klass);
}

void Interpreter::visitLiteral(LiteralExpr& literalExpr) { EXPR_RETURN_FROM_VISIT(literalExpr.getValue()); }

void Interpreter::visitUnaryExpr(UnaryExpr& unaryExpr) {
	EXPR_ACCEPT(unaryExpr.getRhs(), *this, Value::Ptr value);
	std::stringstream ss;
	switch(unaryExpr.getOperator().getType()) {
		case TokenType::BANG: EXPR_RETURN_FROM_VISIT(Value::makePtr<Bool>(!value->isImplicitlyTrue()));
		case TokenType::MINUS:
			if(value->getType() == Value::NativeTypes::Number)
				EXPR_RETURN_FROM_VISIT(*value * Value::makePtr<Number>(-1));
			ss << value->toString() << " is not a number";
			throw RuntimeException(ss.str());
		default: assert(false);
	}
}

void Interpreter::visitVariable(VariableExpr& variableExpr) {
	EXPR_RETURN_FROM_VISIT(m_currentContext->getVar(variableExpr.getName().getValue().asString()).value);
}

void Interpreter::visitBlockStmt(BlockStmt& blockStmt) {
	executeStatementsOnContext(blockStmt.getStatements(), Context::makePtr(m_currentContext));
}

void Interpreter::visitDeclarationStmt(DeclarationStmt& declarationStmt) {
	if(!m_currentContext->isTypeKnown(declarationStmt.getType()))
		throw RuntimeException("Unknown Type " + declarationStmt.getType());
	if(const Expression::Ptr& init = declarationStmt.getInitializer()) {
		EXPR_ACCEPT(init, *this, Value::Ptr res);
		if(!res->hasCorrectTypeForAssignment(declarationStmt.getType()))
			throw ParserException(
					"Given type " + res->getType() + " does not match expected type " + declarationStmt.getType());
		m_currentContext->declareVar(declarationStmt.getIdentifier().getValue().asString(), res,
				declarationStmt.getType(), declarationStmt.isMutable());
	} else
		m_currentContext->declareVar(declarationStmt.getIdentifier().getValue().asString(), Value::makePtr<Nothing>(),
				declarationStmt.getType(), declarationStmt.isMutable());
}

void Interpreter::visitClassDeclarationStmt(ClassDeclarationStmt& classDeclarationStmt) {
	auto name = classDeclarationStmt.getIdentifier().getValue().asString();
	if(!m_currentContext->declareType(name, Klass(classDeclarationStmt)))
		throw RuntimeException("Class " + name + " is already declared");
}

void Interpreter::visitExpressionStmt(ExpressionStmt& expressionStmt) { expressionStmt.getExpr()->accept(*this); }

void Interpreter::visitPrintStmt(PrintStmt& printStmt) {
	EXPR_ACCEPT(printStmt.getExpr(), *this, Value::Ptr value);
	std::cout << value->toString() << std::endl;
}

void Interpreter::visitReturnStmt(ReturnStmt& returnStmt) {
	EXPR_ACCEPT(returnStmt.getExpr(), *this, Value::Ptr returnVal);
	throw ReturnException(returnVal);
}

void Interpreter::executeStatementsOnContext(
		const std::vector<Statement::Ptr>& statements, const Context::Ptr& context) {
	Context::Ptr prev = m_currentContext;
	m_currentContext = context;
	try {
		for(const auto& statement : statements)
			statement->accept(*this);
	} catch(ReturnException& exception) {
		m_currentContext = prev;
		throw ReturnException(exception);
	}
	m_currentContext = prev;
}
}