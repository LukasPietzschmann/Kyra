#include "TAST.hpp"

namespace Kyra {
namespace Typed {

Expression::Expression(RefPtr<AppliedType> type) : m_type(type) {}

const AppliedType& Expression::get_type() const { return *m_type; }

RefPtr<AppliedType> Expression::get_type_shared() const { return m_type; }

ExpressionStatement::ExpressionStatement(RefPtr<Expression> expression) : m_expression(expression) {}

const Expression& ExpressionStatement::get_expression() const { return *m_expression; }

void ExpressionStatement::accept(TASTVisitor& visitor) const { visitor.visit(*this); }

Declaration::Declaration(declid_t declaration_id) : m_declaration_id(declaration_id) {}

declid_t Declaration::get_declaration_id() const { return m_declaration_id; }

void Declaration::accept(TASTVisitor& visitor) const { visitor.visit(*this); }

Block::Block(const std::vector<RefPtr<Statement>>& body) : m_body(body) {}

const std::vector<RefPtr<Statement>>& Block::get_body() const { return m_body; }

void Block::accept(TASTVisitor& visitor) const { visitor.visit(*this); }

Function::Function(declid_t function_declaration, RefPtr<Block> implementation) :
	m_function_declaration(function_declaration), m_implementation(implementation) {}

declid_t Function::get_function_declaration_id() const { return m_function_declaration; }

const Block& Function::get_implementation() const { return *m_implementation; }

void Function::accept(TASTVisitor& visitor) const { visitor.visit(*this); }

Return::Return(RefPtr<Expression> expression) : m_expression(expression) {}

const Expression& Return::get_expression() const { return *m_expression; }

void Return::accept(TASTVisitor& visitor) const { visitor.visit(*this); }

IntLiteral::IntLiteral(RefPtr<AppliedType> type, int literal_value) : Expression(type), m_value(literal_value) {}

int IntLiteral::get_value() const { return m_value; }

void IntLiteral::accept(TASTVisitor& visitor) const { visitor.visit(*this); }

Assignment::Assignment(RefPtr<AppliedType> type, declid_t lhs, RefPtr<Expression> rhs) :
	Expression(type), m_lhs(lhs), m_rhs(rhs) {}

declid_t Assignment::get_lhs() const { return m_lhs; }

const Expression& Assignment::get_rhs() const { return *m_rhs; }

void Assignment::accept(TASTVisitor& visitor) const { visitor.visit(*this); }

BinaryExpression::BinaryExpression(
	RefPtr<AppliedType> type, RefPtr<Expression> lhs, RefPtr<Expression> rhs, Token oper) :
	Expression(type),
	m_lhs(lhs), m_rhs(rhs), m_operator(oper) {}

const Expression& BinaryExpression::get_lhs() const { return *m_lhs; }

const Expression& BinaryExpression::get_rhs() const { return *m_rhs; }

const Token& BinaryExpression::get_operator() const { return m_operator; }

void BinaryExpression::accept(TASTVisitor& visitor) const { visitor.visit(*this); }

Call::Call(RefPtr<AppliedType> type, declid_t function_declaration, const std::vector<RefPtr<Expression>>& arguments) :
	Expression(type), m_fuction_declaration(function_declaration), m_arguments(arguments) {}

declid_t Call::get_function_declaration_id() const { return m_fuction_declaration; }

const std::vector<RefPtr<Expression>>& Call::get_arguments() const { return m_arguments; }

void Call::accept(TASTVisitor& visitor) const { visitor.visit(*this); }

VarQuery::VarQuery(RefPtr<AppliedType> type, declid_t declaration) : Expression(type), m_declaration(declaration) {}

declid_t VarQuery::get_declaration_id() const { return m_declaration; }

void VarQuery::accept(TASTVisitor& visitor) const { visitor.visit(*this); }
}
}
