#include "Interpreter.hpp"

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
		for(const auto& statement : statements) {
			try {
				statement->accept(*this);
			}catch(RuntimeException& exception) {
				if(passThroughExceptions)
					throw RuntimeException(exception);
				std::cout << "[ERROR] " << exception.what() << std::endl;
			}catch(ReturnException& exception) {
				std::cout << "[ERROR] " << "Return Statements can only be used in Functions." << std::endl;
			}
		}
	}catch(ParserException& exception) {
		if(passThroughExceptions)
			throw ParserException(exception);
		std::cout << "[ERROR] " << exception.what() << std::endl;
	}catch(LexerException& exception) {
		if(passThroughExceptions)
			throw LexerException(exception);
		std::cout << "[ERROR] " << exception.what() << std::endl;
	}
}

bool Interpreter::isIncompleteStatement(const std::string& code) {
	try {
		Parser parser(Lexer(code).scanTokens());
		parser.parse();
	}catch(ParserException& exception) {
		return exception.isUnfinished();
	}catch(...) {
		return false;
	}
	return false;
}

Value::Ptr Interpreter::visitAccessExpr(AccessExpr& accessExpr) {
	Value::Ptr owner = accessExpr.getOwner()->accept(*this);
	if(owner->getType() != Value::OBJECT)
		throw RuntimeException("Variable " + accessExpr.getName().getValue().asString() +
				" can't be accessed from an expression, that doesn't provide a scope.");
	return owner->object()->get(accessExpr.getName().getValue().asString()).value;
}

Value::Ptr Interpreter::visitAssignmentExpr(AssignmentExpr& assignmentExpr) {
	const Context::ContextValue& cValue = m_currentContext->get(assignmentExpr.getName().getValue().asString());
	if(!cValue.isMutable)
		throw RuntimeException("Variable " + assignmentExpr.getName().getValue().asString() + " can't be rebound");
	Value::Ptr newValue = assignmentExpr.getNewValue()->accept(*this);
	m_currentContext->mutate(assignmentExpr.getName().getValue().asString(), newValue);
	return newValue;
}

Value::Ptr Interpreter::visitBinaryExpr(BinaryExpr& binaryExpr) {
	Value::Ptr lhs = binaryExpr.getLhs()->accept(*this);
	Value::Ptr rhs = binaryExpr.getRhs()->accept(*this);
	switch(binaryExpr.getOperator().getType()) {
		case TokenType::EQUAL_EQUAL: return Value::makePtr(*lhs == *rhs);
		case TokenType::BANG_EQUAL: return Value::makePtr(*lhs != *rhs);
		case TokenType::GREATER: return Value::makePtr(*lhs > *rhs);
		case TokenType::GREATER_EQUAL: return Value::makePtr(*lhs >= *rhs);
		case TokenType::LESS: return Value::makePtr(*lhs < *rhs);
		case TokenType::LESS_EQUAL: return Value::makePtr(*lhs <= *rhs);
		case TokenType::MINUS:
			if(lhs->getType() != Value::NUMBER || rhs->getType() != Value::NUMBER)
				throw RuntimeException("The - Operator can only operate on two numbers");
			return Value::makePtr(*lhs - *rhs);
		case TokenType::PLUS:
			if(!(lhs->getType() == Value::NUMBER && rhs->getType() == Value::NUMBER) &&
					!(lhs->getType() == Value::STRING && rhs->getType() == Value::STRING))
				throw RuntimeException("The + Operator can only operate on two numbers or two strings");
			return Value::makePtr(*lhs + *rhs);
		case TokenType::STAR:
			if((lhs->getType() != Value::NUMBER && lhs->getType() != Value::STRING) || rhs->getType() != Value::NUMBER)
				throw RuntimeException("The * Operator can only operate on two numbers or a string and a number");
			return Value::makePtr(*lhs * *rhs);
		case TokenType::SLASH:
			if(lhs->getType() != Value::NUMBER || rhs->getType() != Value::NUMBER)
				throw RuntimeException("The / Operator can only operate on two numbers");
			return Value::makePtr(*lhs / *rhs);
		default: assert(false);
	}
}

Value::Ptr Interpreter::visitCallExpr(CallExpr& callExpr) {
	Value::Ptr fun = callExpr.getFunction()->accept(*this);
	unsigned int arity = fun->function()->getArity();

	if(arity != callExpr.getArguments().size())
		throw RuntimeException(
				"The Function needs to be called with " + std::to_string(arity) + " arguments. You provided " +
						std::to_string(callExpr.getArguments().size()) + ".");

	std::vector<Value::Ptr> arguments;
	for(const auto& arg : callExpr.getArguments())
		arguments.push_back(arg->accept(*this));

	Value::Ptr res = fun->function()->exec(arguments);
	return res;
}

Value::Ptr Interpreter::visitFunction(FunctionExpr& functionExpr) {
	return Value::makePtr(Function::makePtr(functionExpr, m_currentContext));
}

Value::Ptr Interpreter::visitGroupExpr(GroupExpr& groupExpr) {
	return groupExpr.getExpr()->accept(*this);
}

Value::Ptr Interpreter::visitLiteral(LiteralExpr& literalExpr) {
	return Value::makePtr(literalExpr.getValue());
}

Value::Ptr Interpreter::visitObject(ObjectExpr& objectExpr) {
	Context::Ptr context = Context::makePtr(m_currentContext);
	const auto& block = std::dynamic_pointer_cast<BlockStmt>(objectExpr.getImplementation());
	executeStatementsOnContext(block->getStatements(), context);
	return Value::makePtr(context);
}

Value::Ptr Interpreter::visitUnaryExpr(UnaryExpr& unaryExpr) {
	Value::Ptr value = unaryExpr.getRhs()->accept(*this);
	std::stringstream ss;
	switch(unaryExpr.getOperator().getType()) {
		case TokenType::BANG: return Value::makePtr(!value->isTrue());
		case TokenType::MINUS:
			if(value->getType() == Value::NUMBER)
				return Value::makePtr(value->number() * -1);
			ss << *value << " is not a number";
			throw RuntimeException(ss.str());
		default: assert(false);
	}
}

Value::Ptr Interpreter::visitVariable(VariableExpr& variableExpr) {
	return m_currentContext->get(variableExpr.getName().getValue().asString()).value;
}

void Interpreter::visitBlockStmt(BlockStmt& blockStmt) {
	executeStatementsOnContext(blockStmt.getStatements(), Context::makePtr(m_currentContext));
}

void Interpreter::visitDeclarationStmt(DeclarationStmt& declarationStmt) {
	if(const Expression::Ptr& init = declarationStmt.getInitializer())
		m_currentContext->declare(declarationStmt.getIdentifier().getValue().asString(), init->accept(*this),
				declarationStmt.isMutable());
	else
		m_currentContext->declare(declarationStmt.getIdentifier().getValue().asString(), Value::makePtr(),
				declarationStmt.isMutable());
}

void Interpreter::visitExpressionStmt(ExpressionStmt& expressionStmt) {
	expressionStmt.getExpr()->accept(*this);
}

void Interpreter::visitPrintStmt(PrintStmt& printStmt) {
	std::cout << *printStmt.getExpr()->accept(*this) << std::endl;
}

void Interpreter::visitReturnStmt(ReturnStmt& returnStmt) {
	Value::Ptr returnVal = returnStmt.getExpr()->accept(*this);
	throw ReturnException(returnVal);
}

void Interpreter::executeStatementsOnContext(const std::vector<Statement::Ptr>& statements,
		const Context::Ptr& context) {
	Context::Ptr prev = m_currentContext;
	m_currentContext = context;
	try {
		for(const auto& statement : statements)
			statement->accept(*this);
	}catch(ReturnException& exception) {
		m_currentContext = prev;
		throw ReturnException(exception);
	}
	m_currentContext = prev;
}
}