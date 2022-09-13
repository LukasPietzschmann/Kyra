#pragma once

#include <llvm/IR/Value.h>

#include "AST.hpp"
#include "Aliases.hpp"

namespace Kyra {
using namespace llvm;

class CodeGen : public ASTVisitor {
	VISIT_RETURN_TYPE(Value*)

public:
	static CodeGen& the() {
		static CodeGen instance;
		return instance;
	}

	CodeGen() = default;
	CodeGen(const CodeGen&) = delete;
	CodeGen(CodeGen&&) noexcept = default;

	CodeGen& operator=(const CodeGen&) = delete;
	CodeGen& operator=(CodeGen&&) noexcept = default;

	void gen_code(const std::vector<RefPtr<Statement>>& statements);

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
};
}
