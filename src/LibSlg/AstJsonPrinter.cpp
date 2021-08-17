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

void AstJsonPrinter::visitAccessExpr(AccessExpr& accessExpr) {
	m_output << R"({"type":"Access","value":{"name":")" << accessExpr.getName().getValue().asString()
			<< R"(","owner":)";
	accessExpr.getOwner()->accept(*this);
	m_output << "}}";
}

void AstJsonPrinter::visitAssignmentExpr(AssignmentExpr& assignmentExpr) {
	m_output << R"({"type":"Assignment","value":{"variable":")" << assignmentExpr.getName().getValue().asString()
			<< R"(","newValue":)";
	assignmentExpr.getNewValue()->accept(*this);
	m_output << "}}";
}

void AstJsonPrinter::visitBinaryExpr(BinaryExpr& binaryExpr) {
	m_output << R"({"type":"Binary","value":{"operator":")" << TokenTypeName::getFor(binaryExpr.getOperator().getType())
			<< R"(","lhs":)";
	binaryExpr.getLhs()->accept(*this);
	m_output << R"(,"rhs":)";
	binaryExpr.getRhs()->accept(*this);
	m_output << "}}";
}

void AstJsonPrinter::visitCallExpr(CallExpr& callExpr) {
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
}

void AstJsonPrinter::visitFunction(Function& functionExpr) {
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
}

void AstJsonPrinter::visitLiteral(Literal& literal) {
	m_output << R"({"type":"Literal","literalType":")" << ValueTypeName::getFor(literal.getValue().getType())
			<< R"(","value":")";
	switch(literal.getValue().getType()) {
		case Value::STRING: m_output << literal.getValue().string();
			break;
		case Value::NUMBER: m_output << literal.getValue().number();
			break;
		case Value::BOOL: m_output << literal.getValue().boolean();
			break;
		case Value::NOTHING: m_output << "nothing";
			break;
	}
	m_output << R"("})";
}

void AstJsonPrinter::visitObject(Object& objectExpr) {
	m_output << R"({"type":"Object","value":{"implementation":)";
	objectExpr.getImplementation()->accept(*this);
	m_output << R"(}})";
}

void AstJsonPrinter::visitUnaryExpr(UnaryExpr& unaryExpr) {
	m_output << R"({"type":"Unary","value":{"operator":")" << TokenTypeName::getFor(unaryExpr.getOperator().getType())
			<< R"(","rhs":)";
	unaryExpr.getRhs()->accept(*this);
	m_output << "}}";
}

void AstJsonPrinter::visitVariable(Variable& variableExpr) {
	m_output << R"({"type":"Variable","value":")" << variableExpr.getName().getValue().asString() << R"("})";
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
	m_output << R"({"type":"Declaration","value":{"variable":")" << declarationStmt.getIdentifier() << R"(")";
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