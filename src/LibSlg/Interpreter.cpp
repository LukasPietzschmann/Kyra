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

		TypeChecker::Result result = TypeChecker::getInstance().check(statements);
		for(const auto& error : result.getErrors()) {
			if(passThroughExceptions)
				throw TypingException(error);
			std::cout << "[Typing Error] " << error << "\n";
		}
		std::cout << std::flush;
		if(result.hasErrors())
			return;

		for(const auto& statement : statements)
			statement->accept(*this);
	} catch(const RuntimeException& exception) {
		if(passThroughExceptions)
			throw RuntimeException(exception);
		std::cout << "[Runtime Error] " << exception.what() << std::endl;
	} catch(const ParserException& exception) {
		if(passThroughExceptions)
			throw ParserException(exception);
		std::cout << "[Parser Error] " << exception.what() << std::endl;
	} catch(const LexerException& exception) {
		if(passThroughExceptions)
			throw LexerException(exception);
		std::cout << "[Lexer Error] " << exception.what() << std::endl;
	}
}

bool Interpreter::isIncompleteStatement(const std::string& code) const {
	try {
		Parser parser(Lexer(code).scanTokens());
		parser.parse();
	} catch(const ParserException& exception) { return exception.isUnfinished(); } catch(...) {
		return false;
	}
	return false;
}

void Interpreter::visitAccessExpr(AccessExpr& accessExpr) {
	EXPR_ACCEPT(accessExpr.getOwner(), *this, Value::Ptr owner);
	const auto& klass = Value::as<Klass>(owner);
	EXPR_RETURN_FROM_VISIT(klass->getInstanceContext()->getVar(accessExpr.getName().getValue().asString()).value);
}

void Interpreter::visitAssignmentExpr(AssignmentExpr& assignmentExpr) {
	RuntimeContext* context = m_currentContext;
	if(assignmentExpr.getOwner() != nullptr) {
		EXPR_ACCEPT(assignmentExpr.getOwner(), *this, Value::Ptr owner);
		context = Value::as<Klass>(owner)->getInstanceContext();
	}

	EXPR_ACCEPT(assignmentExpr.getNewValue(), *this, Value::Ptr newValue);
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
		case TokenType::MINUS: EXPR_RETURN_FROM_VISIT(*lhs - rhs);
		case TokenType::PLUS: EXPR_RETURN_FROM_VISIT(*lhs + rhs);
		case TokenType::STAR: EXPR_RETURN_FROM_VISIT(*lhs * rhs);
		case TokenType::SLASH: EXPR_RETURN_FROM_VISIT(*lhs / rhs);
		default: assert(false);
	}
}

void Interpreter::visitCallExpr(CallExpr& callExpr) {
	EXPR_ACCEPT(callExpr.getFunction(), *this, Value::Ptr fun);

	std::vector<Value::Ptr> arguments;
	for(const auto& arg : callExpr.getArguments()) {
		EXPR_ACCEPT(arg, *this, Value::Ptr res);
		arguments.push_back(res);
	}

	Value::Ptr res = Value::as<Function>(fun)->exec(arguments);
	EXPR_RETURN_FROM_VISIT(res);
}

void Interpreter::visitFunction(FunctionExpr& functionExpr) {
	EXPR_RETURN_FROM_VISIT(Value::makePtr<Function>(functionExpr, *m_currentContext));
}

void Interpreter::visitGroupExpr(GroupExpr& groupExpr) { return groupExpr.getExpr()->accept(*this); }

void Interpreter::visitInstantiationExpr(InstantiationExpr& instantiationExpr) {
	Value::Ptr klass = Value::makePtr<Klass>(m_currentContext->getCustomType(instantiationExpr.getName()));
	std::vector<Value::Ptr> values;
	for(const auto& argument : instantiationExpr.getArguments()) {
		EXPR_ACCEPT(argument, *this, Value::Ptr res);
		values.emplace_back(res);
	}
	Value::as<Klass>(klass)->instantiate(values);
	EXPR_RETURN_FROM_VISIT(klass);
}

void Interpreter::visitLiteral(LiteralExpr& literalExpr) { EXPR_RETURN_FROM_VISIT(literalExpr.getValue()); }

void Interpreter::visitTypeExpr(TypeExpr&) {
	// TypeExpr has no effect during runtime
}

void Interpreter::visitUnaryExpr(UnaryExpr& unaryExpr) {
	EXPR_ACCEPT(unaryExpr.getRhs(), *this, Value::Ptr value);
	switch(unaryExpr.getOperator().getType()) {
		case TokenType::BANG: EXPR_RETURN_FROM_VISIT(Value::makePtr<Bool>(!value->isImplicitlyTrue()));
		case TokenType::MINUS: EXPR_RETURN_FROM_VISIT(*value * Value::makePtr<Number>(-1));
		default: assert(false);
	}
}

void Interpreter::visitVariable(VariableExpr& variableExpr) {
	EXPR_RETURN_FROM_VISIT(m_currentContext->getVar(variableExpr.getName().getValue().asString()).value);
}

void Interpreter::visitBlockStmt(BlockStmt& blockStmt) {
	executeStatementsOnContext(blockStmt.getStatements(), *m_currentContext);
}

void Interpreter::visitDeclarationStmt(DeclarationStmt& declarationStmt) {
	if(const Expression::Ptr& init = declarationStmt.getInitializer()) {
		EXPR_ACCEPT(init, *this, Value::Ptr res);
		m_currentContext->declareVar(declarationStmt.getIdentifier().getValue().asString(),
				res,
				declarationStmt.isMutable());
	} else
		m_currentContext->declareVar(declarationStmt.getIdentifier().getValue().asString(),
				Value::makePtr<Nothing>(),
				declarationStmt.isMutable());
}

void Interpreter::visitClassDeclarationStmt(ClassDeclarationStmt& classDeclarationStmt) {
	auto name = classDeclarationStmt.getIdentifier().getValue().asString();
	m_currentContext->declareType(name, Klass(classDeclarationStmt));
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

void Interpreter::executeStatementsOnContext(const std::vector<Statement::Ptr>& statements, RuntimeContext& context) {
	RuntimeContext* prev = m_currentContext;
	m_currentContext = &context;
	try {
		for(const auto& statement : statements)
			statement->accept(*this);
	} catch(const ReturnException& exception) {
		m_currentContext = prev;
		throw ReturnException(exception);
	}
	m_currentContext = prev;
}
}