#include "TAST.hpp"

#include <utility>

namespace Kyra {
namespace Typed {

Expression::Expression(RefPtr<AppliedType> type) : m_type(std::move(type)) {}

const AppliedType& Expression::get_type() const { return *m_type; }

RefPtr<AppliedType> Expression::get_type_shared() const { return m_type; }

ExpressionStatement::ExpressionStatement(RefPtr<Expression> expression) : m_expression(std::move(expression)) {}

const Expression& ExpressionStatement::get_expression() const { return *m_expression; }

void ExpressionStatement::accept(TASTVisitor& visitor) const { visitor.visit(*this); }

Declaration::Declaration(declid_t declaration_id, RefPtr<Expression> initializer) :
	m_declaration_id(declaration_id), m_initializer(initializer) {}

declid_t Declaration::get_declaration_id() const { return m_declaration_id; }

const Expression* Declaration::get_initializer() const { return m_initializer.get(); }

RefPtr<Expression> Declaration::get_initializer_shared() const { return m_initializer; }

void Declaration::accept(TASTVisitor& visitor) const { visitor.visit(*this); }

Structure::Structure(std::string_view identifier, const std::vector<RefPtr<Declaration>>& declarations) :
	m_identifier(identifier), m_declarations(declarations) {}

std::string_view Structure::get_identifier() const { return m_identifier; }

const std::vector<RefPtr<Declaration>>& Structure::get_declarations() const { return m_declarations; }

void Structure::accept(TASTVisitor& visitor) const { visitor.visit(*this); }

Block::Block(const std::vector<RefPtr<Statement>>& body) : m_body(body) {}

const std::vector<RefPtr<Statement>>& Block::get_body() const { return m_body; }

void Block::accept(TASTVisitor& visitor) const { visitor.visit(*this); }

Function::Function(
	declid_t function_declaration, RefPtr<Block> implementation, const std::vector<declid_t>& parameters) :
	m_function_declaration(function_declaration),
	m_implementation(std::move(implementation)), m_parameters(parameters) {}

declid_t Function::get_function_declaration_id() const { return m_function_declaration; }

const Block& Function::get_implementation() const { return *m_implementation; }

const std::vector<declid_t>& Function::get_parameters() const { return m_parameters; }

void Function::accept(TASTVisitor& visitor) const { visitor.visit(*this); }

Print::Print(RefPtr<Expression> expression) : m_expression(std::move(expression)) {}

const Expression& Print::get_expression() const { return *m_expression; }

void Print::accept(TASTVisitor& visitor) const { visitor.visit(*this); }

Return::Return(RefPtr<Expression> expression) : m_expression(std::move(expression)) {}

const Expression& Return::get_expression() const { return *m_expression; }

void Return::accept(TASTVisitor& visitor) const { visitor.visit(*this); }

IntLiteral::IntLiteral(RefPtr<AppliedType> type, int literal_value) :
	Expression(std::move(type)), m_value(literal_value) {}

int IntLiteral::get_value() const { return m_value; }

void IntLiteral::accept(TASTVisitor& visitor) const { visitor.visit(*this); }

Assignment::Assignment(RefPtr<AppliedType> type, declid_t lhs, RefPtr<Expression> rhs) :
	Expression(std::move(type)), m_lhs(lhs), m_rhs(std::move(rhs)) {}

declid_t Assignment::get_lhs() const { return m_lhs; }

const Expression& Assignment::get_rhs() const { return *m_rhs; }

void Assignment::accept(TASTVisitor& visitor) const { visitor.visit(*this); }

BinaryExpression::BinaryExpression(
	RefPtr<AppliedType> type, RefPtr<Expression> lhs, RefPtr<Expression> rhs, Token oper) :
	Expression(std::move(type)),
	m_lhs(std::move(lhs)), m_rhs(std::move(rhs)), m_operator(oper) {}

const Expression& BinaryExpression::get_lhs() const { return *m_lhs; }

const Expression& BinaryExpression::get_rhs() const { return *m_rhs; }

const Token& BinaryExpression::get_operator() const { return m_operator; }

void BinaryExpression::accept(TASTVisitor& visitor) const { visitor.visit(*this); }

Call::Call(RefPtr<AppliedType> type, declid_t function_declaration, const std::vector<RefPtr<Expression>>& arguments) :
	Expression(std::move(type)), m_fuction_declaration(function_declaration), m_arguments(arguments) {}

declid_t Call::get_function_declaration_id() const { return m_fuction_declaration; }

const std::vector<RefPtr<Expression>>& Call::get_arguments() const { return m_arguments; }

void Call::accept(TASTVisitor& visitor) const { visitor.visit(*this); }

VarQuery::VarQuery(RefPtr<AppliedType> type, declid_t declaration) :
	Expression(std::move(type)), m_declaration(declaration) {}

declid_t VarQuery::get_declaration_id() const { return m_declaration; }

void VarQuery::accept(TASTVisitor& visitor) const { visitor.visit(*this); }
}
}
