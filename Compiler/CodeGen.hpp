#pragma once

#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Value.h>

#include <map>

#include "Aliases.hpp"
#include "TAST.hpp"
#include "Type.hpp"

namespace Kyra {

class CodeGen : public Typed::TASTVisitor {
	VISIT_RETURN_TYPE(llvm::Value*)

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

	void gen_code(const std::vector<RefPtr<Typed::Statement>>& statements);

	void visit(const Typed::ExpressionStatement& expresion_statement) override;
	void visit(const Typed::Declaration& declaration) override;
	void visit(const Typed::Structure& structure) override;
	void visit(const Typed::Function& function) override;
	void visit(const Typed::Print& print_statement) override;
	void visit(const Typed::Return& return_statement) override;
	void visit(const Typed::Block& block) override;
	void visit(const Typed::IntLiteral& literal) override;

	void visit(const Typed::Assignment& assignment) override;
	void visit(const Typed::BinaryExpression& binary_expression) override;
	void visit(const Typed::Call& call) override;
	void visit(const Typed::VarQuery& var_query) override;

private:
	OwnPtr<llvm::LLVMContext> llvm_context;
	OwnPtr<llvm::Module> llvm_module;
	OwnPtr<llvm::IRBuilder<>> ir_builder;

	std::map<declid_t, std::pair<llvm::Value*, unsigned>> m_declarations;
};
}
