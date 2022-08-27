#include "AST.hpp"

ASTNode::ASTNode(const SourceRange& source_range) : m_source_range(source_range){};

const SourceRange& ASTNode::get_source_range() const { return m_source_range; }

Statement::Statement(const SourceRange& source_range) : ASTNode(source_range) {}

Expression::Expression(const SourceRange& source_range) : ASTNode(source_range) {}

ExpressionStatement::ExpressionStatement(const SourceRange& source_range, RefPtr<Expression> expression) :
	Statement(source_range), m_expression(expression) {}

const Expression& ExpressionStatement::get_expression() const { return *m_expression; }

RefPtr<Expression> ExpressionStatement::get_expression_shared() const { return m_expression; }

void ExpressionStatement::accept(ASTVisitor& visitor) const { visitor.visit(*this); }

Declaration::Declaration(const SourceRange& source_range,
		Kind declaration_kind,
		std::string_view identifier,
		RefPtr<Type> type,
		RefPtr<Expression> initializer) :
	Statement(source_range),
	m_declaration_kind(declaration_kind), m_identifier(identifier), m_type(type), m_initializer(initializer) {}

Declaration::Kind Declaration::get_declaration_kind() const { return m_declaration_kind; }

const std::string_view Declaration::get_identifier() const { return m_identifier; }

const Expression& Declaration::get_type() const { return *m_type; }

RefPtr<Expression> Declaration::get_type_shared() const { return m_type; }

const Expression* Declaration::get_initializer() const { return m_initializer.get(); }

RefPtr<Expression> Declaration::get_initializer_shared() const { return m_initializer; }

void Declaration::accept(ASTVisitor& visitor) const { visitor.visit(*this); }

Function::Parameter::Parameter(std::string_view name, RefPtr<Type> type, Declaration::Kind declaration_kind) :
	name(name), type(type), kind(declaration_kind) {}

Function::Function(const SourceRange& source_range,
		std::string_view identifier,
		RefPtr<Statement> body,
		RefPtr<Type> return_type,
		const std::vector<Parameter> parameters) :
	Statement(source_range),
	m_identifier(identifier), m_implementation(body), m_return_type(return_type), m_parameters(parameters) {}

const std::string_view Function::get_identifier() const { return m_identifier; }

const Statement& Function::get_implementation() const { return *m_implementation; }

RefPtr<Statement> Function::get_implementation_shared() const { return m_implementation; }

const Type& Function::get_return_type() const { return *m_return_type; }

RefPtr<Type> Function::get_return_type_shared() const { return m_return_type; }

const std::vector<Function::Parameter>& Function::get_parameters() const { return m_parameters; }

void Function::accept(ASTVisitor& visitor) const { visitor.visit(*this); }

Return::Return(const SourceRange& source_range, RefPtr<Expression> expression) :
	Statement(source_range), m_expression(expression) {}

const Expression& Return::get_expression() const { return *m_expression; }

RefPtr<Expression> Return::get_expression_shared() const { return m_expression; }

void Return::accept(ASTVisitor& visitor) const { visitor.visit(*this); }

Block::Block(const SourceRange& source_range, const std::vector<RefPtr<Statement>>& body) :
	Statement(source_range), m_body(body) {}

const std::vector<RefPtr<Statement>>& Block::get_body() const { return m_body; }

void Block::accept(ASTVisitor& visitor) const { visitor.visit(*this); }

IntLiteral::IntLiteral(const SourceRange& source_range, int literal_value) :
	Expression(source_range), m_value(literal_value) {}

int IntLiteral::get_literal_value() const { return m_value; }

void IntLiteral::accept(ASTVisitor& visitor) const { visitor.visit(*this); }

Assignment::Assignment(const SourceRange& source_range, std::string_view lhs, RefPtr<Expression> rhs) :
	Expression(source_range), m_lhs(lhs), m_rhs(rhs) {}

const std::string_view Assignment::get_lhs() const { return m_lhs; }

const Expression& Assignment::get_rhs() const { return *m_rhs; }

RefPtr<Expression> Assignment::get_rhs_shared() const { return m_rhs; }

void Assignment::accept(ASTVisitor& visitor) const { visitor.visit(*this); }

BinaryExpression::BinaryExpression(const SourceRange& source_range,
		RefPtr<Expression> lhs,
		RefPtr<Expression> rhs,
		Token oper) :
	Expression(source_range),
	m_lhs(lhs), m_rhs(rhs), m_operator(oper) {}

const Expression& BinaryExpression::get_lhs() const { return *m_lhs; }

RefPtr<Expression> BinaryExpression::get_lhs_shared() const { return m_lhs; }

const Expression& BinaryExpression::get_rhs() const { return *m_rhs; }

RefPtr<Expression> BinaryExpression::get_rhs_shared() const { return m_rhs; }

const Token& BinaryExpression::get_operator() const { return m_operator; }

void BinaryExpression::accept(ASTVisitor& visitor) const { visitor.visit(*this); }

Type::Type(const SourceRange& source_range, std::string_view type) : Expression(source_range), m_type(type) {}

const std::string_view Type::get_type() const { return m_type; }

void Type::accept(ASTVisitor& visitor) const { visitor.visit(*this); }

Call::Argument::Argument(RefPtr<Expression> value) : value(value) {}

Call::Call(const SourceRange& source_range, std::string_view function_name, const std::vector<Argument>& arguments) :
	Expression(source_range), m_function_name(function_name), m_arguments(arguments) {}

const std::string_view Call::get_function_name() const { return m_function_name; }

const std::vector<Call::Argument>& Call::get_arguments() const { return m_arguments; }

void Call::accept(ASTVisitor& visitor) const { visitor.visit(*this); }

Group::Group(const SourceRange& source_range, RefPtr<Expression> content) :
	Expression(source_range), m_content(content) {}

const Expression& Group::get_content() const { return *m_content; }

RefPtr<Expression> Group::get_content_shared() const { return m_content; }

void Group::accept(ASTVisitor& visitor) const { visitor.visit(*this); }

VarQuery::VarQuery(const SourceRange& source_range, std::string_view identifier) :
	Expression(source_range), m_identifier(identifier) {}

const std::string_view VarQuery::get_identifier() const { return m_identifier; }

void VarQuery::accept(ASTVisitor& visitor) const { visitor.visit(*this); }
