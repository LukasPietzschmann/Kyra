#pragma once

#include <iostream>
#include <string>

#include "AST.hpp"
#include "Aliases.hpp"

namespace Kyra {

class ASTPrinter : public Untyped::ASTVisitor {
public:
	static ASTPrinter& the() {
		static ASTPrinter instance;
		return instance;
	}

	ASTPrinter() = default;
	ASTPrinter(const ASTPrinter&) = delete;
	ASTPrinter(ASTPrinter&&) noexcept = default;

	ASTPrinter& operator=(const ASTPrinter&) = delete;
	ASTPrinter& operator=(ASTPrinter&&) noexcept = default;

	void print(const Untyped::Statement& statement);

	void visit(const Untyped::ExpressionStatement& expresion_statement) override;
	void visit(const Untyped::Declaration& declaration) override;
	void visit(const Untyped::Structure& structure) override;
	void visit(const Untyped::Function& function) override;
	void visit(const Untyped::Print& print_statement) override;
	void visit(const Untyped::Return& return_statement) override;
	void visit(const Untyped::Block& block) override;
	void visit(const Untyped::IntLiteral& literal) override;

	void visit(const Untyped::Assignment& assignment) override;
	void visit(const Untyped::BinaryExpression& binary_expression) override;
	void visit(const Untyped::TypeIndicator& type) override;
	void visit(const Untyped::Call& call) override;
	void visit(const Untyped::Group& group) override;
	void visit(const Untyped::VarQuery& var_query) override;

private:
	unsigned m_indent;

	template <typename... Args, typename = All<std::string_view, Args...>>
	void print_with_indent(Args... strings) const {
		((std::cout << std::string(m_indent, '\t')) << ... << strings) << '\n';
	}
};
}
