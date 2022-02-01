#include "AstLogger.hpp"

namespace Kyra {
void AstLogger::logStatement(const Statement::Ptr& statement) {
	m_indent = 0;
	statement->accept(*this);
}

void AstLogger::visitBlockStmt(BlockStmt& blockStmt) {
	COUT << "Block" << std::endl;
	++m_indent;
	for(const auto& stmt : blockStmt.getStatements())
		stmt->accept(*this);
	--m_indent;
}

void AstLogger::visitDeclarationStmt(DeclarationStmt& declarationStmt) {
	COUT << "Declaration of variable " << declarationStmt.getIdentifier().getValue().asString();
	if(declarationStmt.getInitializer()) {
		std::cout << " to " << std::endl;
		++m_indent;
		declarationStmt.getInitializer()->accept(*this);
		--m_indent;
	} else
		std::cout << std::endl;
}

void AstLogger::visitClassDeclarationStmt(ClassDeclarationStmt& classDeclarationStmt) {
	COUT << "Declaration of class " << classDeclarationStmt.getIdentifier().getValue().asString() << "\n";
	COUT << "Constructor parameter\n";
	++m_indent;
	std::stringstream parameter;
	for(const auto& param : classDeclarationStmt.getConstructorParameters())
		parameter << param.name.getValue().asString() << " ";
	if(!classDeclarationStmt.getConstructorParameters().empty())
		COUT << parameter.str() << "\n";
	--m_indent;
	std::cout << "Declarations" << std::endl;
	++m_indent;
	for(const auto& decl : classDeclarationStmt.getDeclarations())
		decl->accept(*this);
	--m_indent;
}

void AstLogger::visitExpressionStmt(ExpressionStmt& expressionStmt) {
	COUT << "Expression" << std::endl;
	++m_indent;
	expressionStmt.getExpr()->accept(*this);
	--m_indent;
}

void AstLogger::visitPrintStmt(PrintStmt& printStmt) {
	COUT << "Print" << std::endl;
	++m_indent;
	printStmt.getExpr()->accept(*this);
	--m_indent;
}

void AstLogger::visitReturnStmt(ReturnStmt& returnStmt) {
	COUT << "Return" << std::endl;
	++m_indent;
	returnStmt.getExpr()->accept(*this);
	--m_indent;
}

void AstLogger::visitWhileStmt(WhileStmt& whileStmt) {
	COUT << "While" << std::endl;
	++m_indent;
	whileStmt.getCondition()->accept(*this);
	--m_indent;
	COUT << "Do" << std::endl;
	++m_indent;
	whileStmt.getStatement()->accept(*this);
	--m_indent;
}

void AstLogger::visitAccessExpr(AccessExpr& accessExpr) {
	COUT << "Get Variable " << accessExpr.getName().getValue().asString() << " from " << std::endl;
	++m_indent;
	accessExpr.getOwner()->accept(*this);
	--m_indent;
}

void AstLogger::visitAssignmentExpr(AssignmentExpr& assignmentExpr) {
	COUT << "Assignment of variable " << assignmentExpr.getName().getValue().asString() << " to " << std::endl;
	++m_indent;
	assignmentExpr.getNewValue()->accept(*this);
	--m_indent;
}

void AstLogger::visitBinaryExpr(BinaryExpr& binaryExpr) {
	COUT << "Binary Expr with Operand " << TokenTypeName::getFor(binaryExpr.getOperator().getType()) << std::endl;
	COUT << "Lhs: " << std::endl;
	++m_indent;
	binaryExpr.getLhs()->accept(*this);
	--m_indent;
	COUT << "Rhs: " << std::endl;
	++m_indent;
	binaryExpr.getRhs()->accept(*this);
	--m_indent;
}

void AstLogger::visitCallExpr(CallExpr& callExpr) {
	COUT << "Call to" << std::endl;
	++m_indent;
	callExpr.getFunction()->accept(*this);
	--m_indent;
	COUT << "With arguments" << std::endl;
	++m_indent;
	for(const auto& argument : callExpr.getArguments())
		argument->accept(*this);
	--m_indent;
}

void AstLogger::visitFunction(FunctionExpr& functionExpr) {
	std::stringstream parameters;
	for(const auto& parameter : functionExpr.getParameters())
		parameters << parameter.name.getValue().asString() << " ";
	COUT << "Function" << std::endl;
	COUT << "Parameter " << parameters.str() << std::endl;
	COUT << "Implementation" << std::endl;
	++m_indent;
	functionExpr.getImplementation()->accept(*this);
	--m_indent;
}

void AstLogger::visitGroupExpr(GroupExpr& groupExpr) {
	COUT << "Group" << std::endl;
	++m_indent;
	groupExpr.getExpr()->accept(*this);
	--m_indent;
}

void AstLogger::visitInstantiationExpr(InstantiationExpr& instantiationExpr) {
	COUT << "Instantiation of class " + instantiationExpr.getName() + "\n";
	COUT << "With constructor arguments\n";
	++m_indent;
	for(const auto& argument : instantiationExpr.getArguments())
		argument->accept(*this);
	--m_indent;
}

void AstLogger::visitLiteral(LiteralExpr& literal) {
	COUT << "Literal with value of " << literal.getValue()->toString() << std::endl;
}

void AstLogger::visitTypeExpr(TypeExpr& typeExpr) { COUT << "Type " << typeExpr.getName(); }

void AstLogger::visitUnaryExpr(UnaryExpr& unaryExpr) {
	COUT << "Unary Expr with Operand " << TokenTypeName::getFor(unaryExpr.getOperator().getType()) << std::endl;
	COUT << "Rhs: " << std::endl;
	++m_indent;
	unaryExpr.getRhs()->accept(*this);
	--m_indent;
}
void AstLogger::visitVariable(VariableExpr& variableExpr) {
	COUT << "Variable with identifier " << variableExpr.getName().getValue().asString() << std::endl;
}
}