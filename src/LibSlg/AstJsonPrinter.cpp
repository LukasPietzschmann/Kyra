#include "AstJsonPrinter.hpp"

namespace LibSlg {
std::string AstJsonPrinter::dump() {
	if(m_statements.size() > 1)
		m_output << "[";
	bool needsComma = false;
	for(const auto& statement : m_statements) {
		if(needsComma)
			m_output << ",";
		statement->accept(*this);
		needsComma = true;
	}
	if(m_statements.size() > 1)
		m_output << "]";
	return m_output.str();
}

Value::Ptr AstJsonPrinter::visitAccessExpr(AccessExpr& accessExpr) {
	m_output << R"({"type":"Access","value":{"name":")" << accessExpr.getName().getValue().asString()
			<< R"(","owner":)";
	accessExpr.getOwner()->accept(*this);
	m_output << "}}";
	return nullptr;
}

Value::Ptr AstJsonPrinter::visitAssignmentExpr(AssignmentExpr& assignmentExpr) {
	m_output << R"({"type":"Assignment","value":{"variable":")" << assignmentExpr.getName().getValue().asString()
			<< R"(","newValue":)";
	assignmentExpr.getNewValue()->accept(*this);
	m_output << "}}";
	return nullptr;
}

Value::Ptr AstJsonPrinter::visitBinaryExpr(BinaryExpr& binaryExpr) {
	m_output << R"({"type":"Binary","value":{"operator":")" << TokenTypeName::getFor(binaryExpr.getOperator().getType())
			<< R"(","lhs":)";
	binaryExpr.getLhs()->accept(*this);
	m_output << R"(,"rhs":)";
	binaryExpr.getRhs()->accept(*this);
	m_output << "}}";
	return nullptr;
}

Value::Ptr AstJsonPrinter::visitCallExpr(CallExpr& callExpr) {
	m_output << R"({"type":"Call","value":{"function":)";
	callExpr.getFunction()->accept(*this);
	m_output << R"(,"arguments":[)";
	bool needsComma = false;
	for(const auto& argument : callExpr.getArguments()) {
		if(needsComma)
			m_output << ",";
		argument->accept(*this);
		needsComma = true;
	}
	m_output << R"(]}})";
	return nullptr;
}

Value::Ptr AstJsonPrinter::visitFunction(FunctionExpr& functionExpr) {
	m_output << R"({"type":"Function","value":{"parameters":[)";
	bool needsComma = false;
	for(const auto& parameter : functionExpr.getParameters()) {
		if(needsComma)
			m_output << ",";
		m_output << "\"" << parameter.getValue().asString() << "\"";
		needsComma = true;
	}
	m_output << R"(], "implementation":)";
	functionExpr.getImplementation()->accept(*this);
	m_output << R"(}})";
	return nullptr;
}

Value::Ptr AstJsonPrinter::visitGroupExpr(GroupExpr& groupExpr) {
	m_output << R"({"type":"Group","value":)";
	groupExpr.getExpr()->accept(*this);
	m_output << "}";
	return nullptr;
}

Value::Ptr AstJsonPrinter::visitLiteral(LiteralExpr& literalExpr) {
	m_output << R"({"type":"Literal","literalType":")" << ValueTypeName::getFor(literalExpr.getValue().getType())
			<< R"(","value":")";
	switch(literalExpr.getValue().getType()) {
		case Value::STRING: m_output << literalExpr.getValue().string();
			break;
		case Value::NUMBER: m_output << literalExpr.getValue().number();
			break;
		case Value::BOOL: m_output << literalExpr.getValue().boolean();
			break;
		case Value::NOTHING: m_output << "nothing";
			break;
		case Value::OBJECT: m_output << "object";
			break;
		case Value::FUNCTION: m_output << "function";
			break;
	}
	m_output << R"("})";
	return nullptr;
}

Value::Ptr AstJsonPrinter::visitObject(ObjectExpr& objectExpr) {
	m_output << R"({"type":"Object","value":{"implementation":)";
	objectExpr.getImplementation()->accept(*this);
	m_output << R"(}})";
	return nullptr;
}

Value::Ptr AstJsonPrinter::visitUnaryExpr(UnaryExpr& unaryExpr) {
	m_output << R"({"type":"Unary","value":{"operator":")" << TokenTypeName::getFor(unaryExpr.getOperator().getType())
			<< R"(","rhs":)";
	unaryExpr.getRhs()->accept(*this);
	m_output << "}}";
	return nullptr;
}

Value::Ptr AstJsonPrinter::visitVariable(VariableExpr& variableExpr) {
	m_output << R"({"type":"Variable","value":")" << variableExpr.getName().getValue().asString() << R"("})";
	return nullptr;
}

void AstJsonPrinter::visitBlockStmt(BlockStmt& blockStmt) {
	m_output << R"({"type":"Block","value":{"implementation":[)";
	bool needsComma = false;
	for(const auto& statement : blockStmt.getStatements()) {
		if(needsComma)
			m_output << ",";
		statement->accept(*this);
		needsComma = true;
	}
	m_output << "]}}";
}

void AstJsonPrinter::visitDeclarationStmt(DeclarationStmt& declarationStmt) {
	m_output << R"({"type":"Declaration","value":{"variable":")"
			<< declarationStmt.getIdentifier().getValue().asString() << R"(")";
	if(const Expression::Ptr& init = declarationStmt.getInitializer()) {
		m_output << R"(,"newValue":)";
		init->accept(*this);
	}
	m_output << "}}";
}

void AstJsonPrinter::visitExpressionStmt(ExpressionStmt& expressionStmt) {
	m_output << R"({"type":"Expression","value":)";
	expressionStmt.getExpr()->accept(*this);
	m_output << "}";
}

void AstJsonPrinter::visitPrintStmt(PrintStmt& printStmt) {
	m_output << R"({"type":"Print","value":)";
	printStmt.getExpr()->accept(*this);
	m_output << "}";
}

void AstJsonPrinter::visitReturnStmt(ReturnStmt& returnStmt) {
	m_output << R"({"type":"Return","value":)";
	returnStmt.getExpr()->accept(*this);
	m_output << "}";
}
}