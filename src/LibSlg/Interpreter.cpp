#include "Interpreter.hpp"

namespace LibSlg {
Interpreter& Interpreter::getInstance() {
	static Interpreter instance;
	return instance;
}

void Interpreter::execute(const std::string& code) {
	Lexer lexer(code);
	auto tokens = lexer.scanTokens();

	Parser parser(tokens);
	try {
		auto statements = parser.parse();
	}catch(ParserException& exception) {
		std::cout << "[ERROR] " << exception.what() << std::endl;
	}
}

void Interpreter::visitAccessExpr(AccessExpr& accessExpr) {

}

void Interpreter::visitAssignmentExpr(AssignmentExpr& assignmentExpr) {

}

void Interpreter::visitBinaryExpr(BinaryExpr& binaryExpr) {

}

void Interpreter::visitCallExpr(CallExpr& callExpr) {

}

void Interpreter::visitFunction(Function& functionExpr) {

}

void Interpreter::visitLiteral(Literal& literal) {

}

void Interpreter::visitObject(Object& objectExpr) {

}

void Interpreter::visitUnaryExpr(UnaryExpr& unaryExpr) {

}

void Interpreter::visitVariable(Variable& variableExpr) {

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