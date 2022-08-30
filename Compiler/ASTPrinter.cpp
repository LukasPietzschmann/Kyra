#include "ASTPrinter.hpp"

void ASTPrinter::print(const Statement& statement) {
	m_indent = 0;
	statement.accept(*this);
	std::cout.flush();
}

void ASTPrinter::visit(const ExpressionStatement& expresion_statement) {
	print_with_indent("Expression Statement:");
	++m_indent;
	expresion_statement.get_expression().accept(*this);
	--m_indent;
}

void ASTPrinter::visit(const Declaration& declaration) {
	print_with_indent("Declaration of ", declaration.get_identifier(), ":");
	if(const Expression* init = declaration.get_initializer(); init != nullptr) {
		++m_indent;
		init->accept(*this);
		--m_indent;
	}
}

void ASTPrinter::visit(const Function& function) {
	print_with_indent("Function ", function.get_identifier(), ":");
	++m_indent;
	function.get_implementation().accept(*this);
	--m_indent;
}

void ASTPrinter::visit(const Return& return_statement) {
	print_with_indent("Return");
	++m_indent;
	return_statement.get_expression().accept(*this);
	--m_indent;
}

void ASTPrinter::visit(const Block& block) {
	print_with_indent("Block");
	++m_indent;
	for(const RefPtr<Statement>& statement : block.get_body())
		statement->accept(*this);
	--m_indent;
}

void ASTPrinter::visit(const IntLiteral&) { print_with_indent("Literal"); }

void ASTPrinter::visit(const Assignment& assignment) {
	print_with_indent("Assignment to ", assignment.get_lhs(), ":");
	++m_indent;
	assignment.get_rhs().accept(*this);
	--m_indent;
}

void ASTPrinter::visit(const BinaryExpression& binary_expression) {
	print_with_indent("Binary Expression:");
	++m_indent;
	binary_expression.get_lhs().accept(*this);
	print_with_indent(binary_expression.get_operator().get_lexeme());
	binary_expression.get_rhs().accept(*this);
	--m_indent;
}

void ASTPrinter::visit(const TypeIndicator& type) { print_with_indent("Type ", type.get_type(), ""); }

void ASTPrinter::visit(const Call& call) { print_with_indent("Call to ", call.get_function_name()); }

void ASTPrinter::visit(const Group& group) {
	print_with_indent("Group:");
	++m_indent;
	group.get_content().accept(*this);
	--m_indent;
}

void ASTPrinter::visit(const VarQuery& var_query) { print_with_indent("Variable ", var_query.get_identifier()); }
