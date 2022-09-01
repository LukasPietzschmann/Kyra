#pragma once

#include <iostream>
#include <string>

#include "AST.hpp"
#include "Aliases.hpp"

namespace Kyra {

class ASTPrinter : public ASTVisitor {
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

	void print(const Statement& statement);

	void visit(const ExpressionStatement& expresion_statement) override;
	void visit(const Declaration& declaration) override;
	void visit(const Function& function) override;
	void visit(const Return& return_statement) override;
	void visit(const Block& block) override;
	void visit(const IntLiteral& literal) override;

	void visit(const Assignment& assignment) override;
	void visit(const BinaryExpression& binary_expression) override;
	void visit(const TypeIndicator& type) override;
	void visit(const Call& call) override;
	void visit(const Group& group) override;
	void visit(const VarQuery& var_query) override;

private:
	unsigned m_indent;

	template <typename... Args, typename = All<std::string_view, Args...>>
	void print_with_indent(Args... strings) const {
		((std::cout << std::string(m_indent, '\t')) << ... << strings) << '\n';
	}
};
}
