#pragma once

#include <vector>

#include "Aliases.hpp"
#include "SourceRange.hpp"
#include "Token.hpp"

class Type;
class ASTVisitor;

class ASTNode {
public:
	template <typename T>
	using Ptr = std::shared_ptr<T>;

	virtual ~ASTNode() = default;

	const SourceRange& get_source_range() const;

	virtual void accept(ASTVisitor& visitor) const = 0;

protected:
	ASTNode(const SourceRange& source_range);

private:
	const SourceRange m_source_range;
};

class Statement : public ASTNode {
protected:
	explicit Statement(const SourceRange& source_range);
};

class Expression : public ASTNode {
protected:
	explicit Expression(const SourceRange& source_range);
};

class ExpressionStatement : public Statement {
public:
	ExpressionStatement(const SourceRange& source_range, RefPtr<Expression> expression);

	const Expression& get_expression() const;
	RefPtr<Expression> get_expression_shared() const;

	void accept(ASTVisitor& visitor) const override;

private:
	RefPtr<Expression> m_expression;
};

class Declaration : public Statement {
public:
	enum class Kind { VAR, VAL };

	Declaration(const SourceRange& source_range,
			Kind declaration_kind,
			std::string_view identifier,
			RefPtr<Type> type,
			RefPtr<Expression> initializer = nullptr);

	Kind get_declaration_kind() const;
	const std::string_view get_identifier() const;
	const Expression& get_type() const;
	RefPtr<Expression> get_type_shared() const;
	const Expression* get_initializer() const;
	RefPtr<Expression> get_initializer_shared() const;

	void accept(ASTVisitor& visitor) const override;

private:
	const Kind m_declaration_kind;
	const std::string_view m_identifier;
	RefPtr<Type> m_type;
	RefPtr<Expression> m_initializer;
};

class Function : public Statement {
public:
	struct Parameter {
		Parameter(std::string_view name, RefPtr<Type> type, Declaration::Kind declaration_kind);
		const std::string_view name;
		RefPtr<Type> type;
		const Declaration::Kind kind;
	};

	Function(const SourceRange& source_range,
			std::string_view identifier,
			RefPtr<Statement> body,
			RefPtr<Type> return_type,
			const std::vector<Parameter> parameters);

	const std::string_view get_identifier() const;
	const Statement& get_implementation() const;
	RefPtr<Statement> get_implementation_shared() const;
	const Type& get_return_type() const;
	RefPtr<Type> get_return_type_shared() const;
	const std::vector<Parameter>& get_parameters() const;

	void accept(ASTVisitor& visitor) const override;

private:
	const std::string_view m_identifier;
	RefPtr<Statement> m_implementation;
	RefPtr<Type> m_return_type;
	const std::vector<Parameter> m_parameters;
};

class Return : public Statement {
public:
	Return(const SourceRange& source_range, RefPtr<Expression> expression);

	const Expression& get_expression() const;
	RefPtr<Expression> get_expression_shared() const;

	void accept(ASTVisitor& visitor) const override;

private:
	RefPtr<Expression> m_expression;
};

class Block : public Statement {
public:
	Block(const SourceRange& source_range, const std::vector<RefPtr<Statement>>& body);

	const std::vector<RefPtr<Statement>>& get_body() const;

	void accept(ASTVisitor& visitor) const override;

private:
	const std::vector<RefPtr<Statement>> m_body;
};

class IntLiteral : public Expression {
public:
	IntLiteral(const SourceRange& source_range, int literal_value);

	int get_literal_value() const;

	void accept(ASTVisitor& visitor) const override;

private:
	const int m_value;
};

class Assignment : public Expression {
public:
	Assignment(const SourceRange& source_range, std::string_view lhs, RefPtr<Expression> rhs);

	const std::string_view get_lhs() const;
	const Expression& get_rhs() const;
	RefPtr<Expression> get_rhs_shared() const;

	void accept(ASTVisitor& visitor) const override;

private:
	const std::string_view m_lhs;
	RefPtr<Expression> m_rhs;
};

class BinaryExpression : public Expression {
public:
	BinaryExpression(const SourceRange& source_range, RefPtr<Expression> lhs, RefPtr<Expression> rhs, Token oper);

	const Expression& get_lhs() const;
	RefPtr<Expression> get_lhs_shared() const;
	const Expression& get_rhs() const;
	RefPtr<Expression> get_rhs_shared() const;
	const Token& get_operator() const;

	void accept(ASTVisitor& visitor) const override;

private:
	RefPtr<Expression> m_lhs;
	RefPtr<Expression> m_rhs;
	const Token m_operator;
};

class Type : public Expression {
public:
	Type(const SourceRange& source_range, std::string_view type);

	const std::string_view get_type() const;

	void accept(ASTVisitor& visitor) const override;

private:
	const std::string_view m_type;
};

class Call : public Expression {
public:
	struct Argument {
		Argument(RefPtr<Expression> value);
		RefPtr<Expression> value;
	};

	Call(const SourceRange& source_range, std::string_view function_name, const std::vector<Argument>& arguments);

	const std::string_view get_function_name() const;
	const std::vector<Argument>& get_arguments() const;

	void accept(ASTVisitor& visitor) const override;

private:
	const std::string_view m_function_name;
	const std::vector<Argument> m_arguments;
};

class Group : public Expression {
public:
	Group(const SourceRange& source_range, RefPtr<Expression> content);

	const Expression& get_content() const;
	RefPtr<Expression> get_content_shared() const;

	void accept(ASTVisitor& visitor) const override;

private:
	RefPtr<Expression> m_content;
};

class VarQuery : public Expression {
public:
	VarQuery(const SourceRange& source_range, std::string_view identifier);

	const std::string_view get_identifier() const;

	void accept(ASTVisitor& visitor) const override;

private:
	const std::string_view m_identifier;
};

class ASTVisitor {
public:
	virtual void visit(const ExpressionStatement& expresion_statement) = 0;
	virtual void visit(const Declaration& declaration) = 0;
	virtual void visit(const Function& function) = 0;
	virtual void visit(const Return& return_statement) = 0;
	virtual void visit(const Block& block) = 0;
	virtual void visit(const IntLiteral& literal) = 0;

	virtual void visit(const Assignment& assignment) = 0;
	virtual void visit(const BinaryExpression& binary_expression) = 0;
	virtual void visit(const Type& type) = 0;
	virtual void visit(const Call& call) = 0;
	virtual void visit(const Group& group) = 0;
	virtual void visit(const VarQuery& var_query) = 0;
};