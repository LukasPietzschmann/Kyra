#include "Interpreter.hpp"

namespace LibSlg {
Interpreter& Interpreter::getInstance() {
	static Interpreter instance;
	return instance;
}

void Interpreter::execute(const std::string& code, bool verboseLogging) {
	Lexer lexer(code);
	auto tokens = lexer.scanTokens();

	Parser parser(tokens);
	try {
		auto statements = parser.parse();
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
				std::cout << "[ERROR] " << exception.what() << std::endl;
			}
		}
	}catch(ParserException& exception) {
		std::cout << "[ERROR] " << exception.what() << std::endl;
	}
}

Value::Ptr Interpreter::visitAccessExpr(AccessExpr& accessExpr) {
	return {};
}

Value::Ptr Interpreter::visitAssignmentExpr(AssignmentExpr& assignmentExpr) {
	Value::Ptr newValue = assignmentExpr.getNewValue()->accept(*this);
	m_global.mutate(assignmentExpr.getName().getValue().asString(), newValue);
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
			return Value::makePtr(*lhs + *rhs);;
		case TokenType::STAR:
			if((lhs->getType() != Value::NUMBER && lhs->getType() != Value::STRING) || rhs->getType() != Value::NUMBER)
				throw RuntimeException("The - Operator can only operate on two numbers or a string and a number");
			return Value::makePtr(*lhs * *rhs);
		case TokenType::SLASH:
			if(lhs->getType() != Value::NUMBER || rhs->getType() != Value::NUMBER)
				throw RuntimeException("The / Operator can only operate on two numbers");
			return Value::makePtr(*lhs / *rhs);;
		default: assert(false);
	}
}

Value::Ptr Interpreter::visitCallExpr(CallExpr& callExpr) {
	return {};
}

Value::Ptr Interpreter::visitFunction(Function& functionExpr) {
	return {};
}

Value::Ptr Interpreter::visitGroupExpr(GroupExpr& groupExpr) {
	return groupExpr.getExpr()->accept(*this);
}

Value::Ptr Interpreter::visitLiteral(Literal& literal) {
	return Value::makePtr(literal.getValue());
}

Value::Ptr Interpreter::visitObject(Object& objectExpr) {
	return {};
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

Value::Ptr Interpreter::visitVariable(Variable& variableExpr) {
	return m_global.get(variableExpr.getName().getValue().asString());
}

void Interpreter::visitBlockStmt(BlockStmt& blockStmt) {
	for(const auto& statement : blockStmt.getStatements())
		statement->accept(*this);
}

void Interpreter::visitDeclarationStmt(DeclarationStmt& declarationStmt) {
	if(const Expression::Ptr& init = declarationStmt.getInitializer())
		m_global.declare(declarationStmt.getIdentifier().getValue().asString(), init->accept(*this));
	else
		m_global.declare(declarationStmt.getIdentifier().getValue().asString());
}

void Interpreter::visitExpressionStmt(ExpressionStmt& expressionStmt) {
	expressionStmt.getExpr()->accept(*this);
}

void Interpreter::visitPrintStmt(PrintStmt& printStmt) {
	std::cout << *printStmt.getExpr()->accept(*this) << std::endl;
}

void Interpreter::visitReturnStmt(ReturnStmt& returnStmt) {

}
}