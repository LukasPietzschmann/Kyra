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
	}catch(ParserException& exception) {
		std::cout << "[ERROR] " << exception.what() << std::endl;
	}
}

Value::Ptr Interpreter::visitAccessExpr(AccessExpr& accessExpr) {
	return {};
}

Value::Ptr Interpreter::visitAssignmentExpr(AssignmentExpr& assignmentExpr) {
	return {};
}

Value::Ptr Interpreter::visitBinaryExpr(BinaryExpr& binaryExpr) {
	return {};
}

Value::Ptr Interpreter::visitCallExpr(CallExpr& callExpr) {
	return {};
}

Value::Ptr Interpreter::visitFunction(Function& functionExpr) {
	return {};
}

Value::Ptr Interpreter::visitGroupExpr(GroupExpr& groupExpr) {
	return {};
}

Value::Ptr Interpreter::visitLiteral(Literal& literal) {
	return {};
}

Value::Ptr Interpreter::visitObject(Object& objectExpr) {
	return {};
}

Value::Ptr Interpreter::visitUnaryExpr(UnaryExpr& unaryExpr) {
	return {};
}

Value::Ptr Interpreter::visitVariable(Variable& variableExpr) {
	return {};
}

void Interpreter::visitBlockStmt(BlockStmt& blockStmt) {

}

void Interpreter::visitDeclarationStmt(DeclarationStmt& declarationStmt) {

}

void Interpreter::visitExpressionStmt(ExpressionStmt& expressionStmt) {

}

void Interpreter::visitPrintStmt(PrintStmt& printStmt) {

}

void Interpreter::visitReturnStmt(ReturnStmt& returnStmt) {

}
}