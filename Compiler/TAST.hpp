#pragma once

#include <vector>

#include "Aliases.hpp"
#include "Token.hpp"
#include "Type.hpp"

namespace Kyra {
namespace Typed {

class TASTVisitor;

class TASTNode {
public:
	virtual ~TASTNode() = default;

	virtual void accept(TASTVisitor& visitor) const = 0;
};

class Statement : public TASTNode {};

class Expression : public TASTNode {
public:
	const AppliedType& get_type() const;
	RefPtr<AppliedType> get_type_shared() const;

protected:
	explicit Expression(RefPtr<AppliedType> type);

	RefPtr<AppliedType> m_type;
};

class ExpressionStatement : public Statement {
public:
	ExpressionStatement(RefPtr<Expression> expression);

	const Expression& get_expression() const;

	void accept(TASTVisitor& visitor) const override;

private:
	RefPtr<Expression> m_expression;
};

class Declaration : public Statement {
public:
	Declaration(declid_t declaration_id);

	declid_t get_declaration_id() const;

	void accept(TASTVisitor& visitor) const override;

private:
	const declid_t m_declaration_id;
};

class Block : public Statement {
public:
	Block(const std::vector<RefPtr<Statement>>& body);

	const std::vector<RefPtr<Statement>>& get_body() const;

	void accept(TASTVisitor& visitor) const override;

private:
	const std::vector<RefPtr<Statement>> m_body;
};

class Function : public Statement {
public:
	Function(declid_t function_declaration, RefPtr<Block> implementation, const std::vector<declid_t>& parameters);

	declid_t get_function_declaration_id() const;
	const Block& get_implementation() const;
	const std::vector<declid_t>& get_parameters() const;

	void accept(TASTVisitor& visitor) const override;

private:
	const declid_t m_function_declaration;
	RefPtr<Block> m_implementation;
	const std::vector<declid_t> m_parameters;
};

class Return : public Statement {
public:
	Return(RefPtr<Expression> expression);

	const Expression& get_expression() const;

	void accept(TASTVisitor& visitor) const override;

private:
	RefPtr<Expression> m_expression;
};

class IntLiteral : public Expression {
public:
	IntLiteral(RefPtr<AppliedType> type, int literal_value);

	int get_value() const;

	void accept(TASTVisitor& visitor) const override;

private:
	const int m_value;
};

class Assignment : public Expression {
public:
	Assignment(RefPtr<AppliedType> type, declid_t lhs, RefPtr<Expression> rhs);

	declid_t get_lhs() const;
	const Expression& get_rhs() const;

	void accept(TASTVisitor& visitor) const override;

private:
	const declid_t m_lhs;
	RefPtr<Expression> m_rhs;
};

class BinaryExpression : public Expression {
public:
	BinaryExpression(RefPtr<AppliedType> type, RefPtr<Expression> lhs, RefPtr<Expression> rhs, Token oper);

	const Expression& get_lhs() const;
	const Expression& get_rhs() const;
	const Token& get_operator() const;

	void accept(TASTVisitor& visitor) const override;

private:
	RefPtr<Expression> m_lhs;
	RefPtr<Expression> m_rhs;
	const Token m_operator;
};

class Call : public Expression {
public:
	Call(RefPtr<AppliedType> type, declid_t function_declaration, const std::vector<RefPtr<Expression>>& arguments);

	declid_t get_function_declaration_id() const;
	const std::vector<RefPtr<Expression>>& get_arguments() const;

	void accept(TASTVisitor& visitor) const override;

private:
	const declid_t m_fuction_declaration;
	const std::vector<RefPtr<Expression>> m_arguments;
};

class VarQuery : public Expression {
public:
	VarQuery(RefPtr<AppliedType> type, declid_t declaration);

	declid_t get_declaration_id() const;

	void accept(TASTVisitor& visitor) const override;

private:
	const declid_t m_declaration;
};

class TASTVisitor {
public:
	virtual ~TASTVisitor() = default;

	virtual void visit(const ExpressionStatement& expresion_statement) = 0;
	virtual void visit(const Declaration& declaration) = 0;
	virtual void visit(const Function& function) = 0;
	virtual void visit(const Return& return_statement) = 0;
	virtual void visit(const Block& block) = 0;
	virtual void visit(const IntLiteral& literal) = 0;

	virtual void visit(const Assignment& assignment) = 0;
	virtual void visit(const BinaryExpression& binary_expression) = 0;
	virtual void visit(const Call& call) = 0;
	virtual void visit(const VarQuery& var_query) = 0;
};
}
}
