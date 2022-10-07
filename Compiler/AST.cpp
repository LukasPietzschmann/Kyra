#include "AST.hpp"

#include <utility>

namespace Kyra {
namespace Untyped {

ASTNode::ASTNode(const SourceRange& source_range) : m_source_range(source_range){};

const SourceRange& ASTNode::get_source_range() const { return m_source_range; }

Statement::Statement(const SourceRange& source_range) : ASTNode(source_range) {}

Expression::Expression(const SourceRange& source_range) : ASTNode(source_range) {}

ExpressionStatement::ExpressionStatement(const SourceRange& source_range, RefPtr<Expression> expression) :
	Statement(source_range), m_expression(std::move(expression)) {}

const Expression& ExpressionStatement::get_expression() const { return *m_expression; }

RefPtr<Expression> ExpressionStatement::get_expression_shared() const { return m_expression; }

void ExpressionStatement::accept(ASTVisitor& visitor) const { visitor.visit(*this); }

Declaration::Declaration(const SourceRange& source_range, Kind declaration_kind, const Token& identifier,
	RefPtr<TypeIndicator> type, RefPtr<Expression> initializer) :
	Statement(source_range),
	m_declaration_kind(declaration_kind), m_identifier(identifier), m_type(std::move(type)),
	m_initializer(std::move(initializer)) {}

Declaration::Kind Declaration::get_declaration_kind() const { return m_declaration_kind; }

const Token& Declaration::get_identifier() const { return m_identifier; }

const TypeIndicator& Declaration::get_type() const { return *m_type; }

RefPtr<TypeIndicator> Declaration::get_type_shared() const { return m_type; }

const Expression* Declaration::get_initializer() const { return m_initializer.get(); }

RefPtr<Expression> Declaration::get_initializer_shared() const { return m_initializer; }

void Declaration::accept(ASTVisitor& visitor) const { visitor.visit(*this); }

Structure::Structure(
	const SourceRange& source_range, const Token& identifier, const std::vector<RefPtr<Declaration>>& declarations) :
	Statement(source_range),
	m_identifier(identifier), m_declarations(declarations) {}

const Token& Structure::get_identifier() const { return m_identifier; }

const std::vector<RefPtr<Declaration>>& Structure::get_declarations() const { return m_declarations; }

void Structure::accept(ASTVisitor& visitor) const { visitor.visit(*this); }

Block::Block(const SourceRange& source_range, const std::vector<RefPtr<Statement>>& body) :
	Statement(source_range), m_body(body) {}

const std::vector<RefPtr<Statement>>& Block::get_body() const { return m_body; }

void Block::accept(ASTVisitor& visitor) const { visitor.visit(*this); }

Function::Parameter::Parameter(
	const Token& identifier, RefPtr<TypeIndicator> type, Declaration::Kind declaration_kind) :
	identifier(identifier),
	type(std::move(type)), kind(declaration_kind) {}

Function::Function(const SourceRange& source_range, const Token& identifier, RefPtr<Block> body,
	RefPtr<TypeIndicator> return_type, const std::vector<Parameter>& parameters) :
	Statement(source_range),
	m_identifier(identifier), m_implementation(std::move(body)), m_return_type(std::move(return_type)),
	m_parameters(parameters) {}

const Token& Function::get_identifier() const { return m_identifier; }

const Block& Function::get_implementation() const { return *m_implementation; }

RefPtr<Block> Function::get_implementation_shared() const { return m_implementation; }

const TypeIndicator& Function::get_return_type() const { return *m_return_type; }

RefPtr<TypeIndicator> Function::get_return_type_shared() const { return m_return_type; }

const std::vector<Function::Parameter>& Function::get_parameters() const { return m_parameters; }

void Function::accept(ASTVisitor& visitor) const { visitor.visit(*this); }

Print::Print(const SourceRange& source_range, RefPtr<Expression> expression) :
	Statement(source_range), m_expression(std::move(expression)) {}

const Expression& Print::get_expression() const { return *m_expression; }

RefPtr<Expression> Print::get_expression_shared() const { return m_expression; }

void Print::accept(ASTVisitor& visitor) const { visitor.visit(*this); }

Return::Return(const SourceRange& source_range, RefPtr<Expression> expression) :
	Statement(source_range), m_expression(std::move(expression)) {}

const Expression& Return::get_expression() const { return *m_expression; }

RefPtr<Expression> Return::get_expression_shared() const { return m_expression; }

void Return::accept(ASTVisitor& visitor) const { visitor.visit(*this); }

IntLiteral::IntLiteral(const SourceRange& source_range, int literal_value) :
	Expression(source_range), m_value(literal_value) {}

int IntLiteral::get_literal_value() const { return m_value; }

void IntLiteral::accept(ASTVisitor& visitor) const { visitor.visit(*this); }

Assignment::Assignment(const SourceRange& source_range, const Token& lhs, RefPtr<Expression> rhs) :
	Expression(source_range), m_lhs(lhs), m_rhs(std::move(rhs)) {}

const Token& Assignment::get_lhs() const { return m_lhs; }

const Expression& Assignment::get_rhs() const { return *m_rhs; }

RefPtr<Expression> Assignment::get_rhs_shared() const { return m_rhs; }

void Assignment::accept(ASTVisitor& visitor) const { visitor.visit(*this); }

BinaryExpression::BinaryExpression(
	const SourceRange& source_range, RefPtr<Expression> lhs, RefPtr<Expression> rhs, Token oper) :
	Expression(source_range),
	m_lhs(std::move(lhs)), m_rhs(std::move(rhs)), m_operator(oper) {}

const Expression& BinaryExpression::get_lhs() const { return *m_lhs; }

RefPtr<Expression> BinaryExpression::get_lhs_shared() const { return m_lhs; }

const Expression& BinaryExpression::get_rhs() const { return *m_rhs; }

RefPtr<Expression> BinaryExpression::get_rhs_shared() const { return m_rhs; }

const Token& BinaryExpression::get_operator() const { return m_operator; }

void BinaryExpression::accept(ASTVisitor& visitor) const { visitor.visit(*this); }

TypeIndicator::TypeIndicator(const SourceRange& source_range, const Token& type) :
	Expression(source_range), m_type(type) {}

const Token& TypeIndicator::get_type() const { return m_type; }

void TypeIndicator::accept(ASTVisitor& visitor) const { visitor.visit(*this); }

Call::Argument::Argument(RefPtr<Expression> value) : value(std::move(value)) {}

Call::Call(const SourceRange& source_range, const Token& function_name, const std::vector<Argument>& arguments) :
	Expression(source_range), m_function_name(function_name), m_arguments(arguments) {}

const Token& Call::get_function_name() const { return m_function_name; }

const std::vector<Call::Argument>& Call::get_arguments() const { return m_arguments; }

void Call::accept(ASTVisitor& visitor) const { visitor.visit(*this); }

Group::Group(const SourceRange& source_range, RefPtr<Expression> content) :
	Expression(source_range), m_content(std::move(content)) {}

const Expression& Group::get_content() const { return *m_content; }

RefPtr<Expression> Group::get_content_shared() const { return m_content; }

void Group::accept(ASTVisitor& visitor) const { visitor.visit(*this); }

VarQuery::VarQuery(const SourceRange& source_range, const Token& identifier, RefPtr<Expression> owner) :
	Expression(source_range), m_identifier(identifier), m_owner(owner) {}

const Token& VarQuery::get_identifier() const { return m_identifier; }

const Expression* VarQuery::get_owner() const { return m_owner.get(); }

RefPtr<Expression> VarQuery::get_owner_shared() const { return m_owner; }

void VarQuery::accept(ASTVisitor& visitor) const { visitor.visit(*this); }
}
}
