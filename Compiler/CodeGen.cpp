#include "CodeGen.hpp"

#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/CallingConv.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/GlobalVariable.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/Verifier.h>

#include <cassert>
#include <string_view>

#include "Plattform.hpp"
#include "Token.hpp"

using namespace llvm;

namespace Kyra {
namespace Utils {

PointerType* get_ptr_type(Type* underlying_type, unsigned indirections = 1) {
	assert(indirections >= 1);
	if(indirections == 1)
		return PointerType::get(underlying_type, 0);
	return PointerType::get(get_ptr_type(underlying_type, indirections - 1), 0);
}
IntegerType* get_integer_type(LLVMContext& context, unsigned width) { return IntegerType::get(context, width); }

ConstantInt* get_integer_constant(const Module& module, int constant, unsigned width, bool is_signed = true) {
	return ConstantInt::get(module.getContext(), APInt(width, constant, is_signed));
}

Type* get_llvm_type_for(LLVMContext& context, const DeclaredType& type) {
	switch(type.get_kind()) {
		case DeclaredType::Integer: return get_integer_type(context, static_cast<const IntType&>(type).get_width());
		case DeclaredType::Function:
		default: assert_not_reached();
	}
}

template <typename Lambda>
void generate_on_basic_block(
	IRBuilder<>& ir_builder, BasicBlock* basic_block, Lambda lambda, bool reset_insert_point = false) {
	BasicBlock* prev_insert_block = ir_builder.GetInsertBlock();
	ir_builder.SetInsertPoint(basic_block);
	lambda();
	if(reset_insert_point)
		ir_builder.SetInsertPoint(prev_insert_block);
}
}

namespace PredefFunctionNames {
static const char* const main = "main";
}

namespace PredefFunctions {
Function* main(Module& module) {
	if(Function* main_function = module.getFunction(PredefFunctionNames::main))
		return main_function;

	llvm::FunctionType* main_function_type =
		llvm::FunctionType::get(Utils::get_integer_type(module.getContext(), C_INT_BIT_WIDTH), false);
	Function* main_function =
		Function::Create(main_function_type, Function::LinkOnceAnyLinkage, PredefFunctionNames::main, module);

	return main_function;
}
}

using namespace Typed;

void CodeGen::gen_code(const std::vector<RefPtr<Statement>>& statements) {
	llvm_context = mk_own<LLVMContext>();
	// TODO: set filename as module name
	llvm_module = mk_own<Module>("Kyra", *llvm_context);
	ir_builder = mk_own<IRBuilder<>>(*llvm_context);

	BasicBlock* main_entry = BasicBlock::Create(llvm_module->getContext());
	PredefFunctions::main(*llvm_module)->getBasicBlockList().push_back(main_entry);
	Utils::generate_on_basic_block(
		*ir_builder, main_entry,
		[&]() {
			for(const RefPtr<Statement>& statement : statements)
				statement->accept(*this);
			ir_builder->CreateRet(Utils::get_integer_constant(*llvm_module, 0, C_INT_BIT_WIDTH));
		},
		true);
	verifyFunction(*PredefFunctions::main(*llvm_module), &errs());
	verifyModule(*llvm_module, &errs());
	llvm_module->print(outs(), nullptr);
}

void CodeGen::visit(const ExpressionStatement& expresion_statement) {
	expresion_statement.get_expression().accept(*this);
}

void CodeGen::visit(const Declaration& declaration) {
	auto [name, type] = DeclarationDumpster::the().retrieve(declaration.get_declaration_id());
	Type* llvm_type = Utils::get_llvm_type_for(llvm_module->getContext(), type->get_declared_type());

	Value* var = nullptr;
	if(ir_builder->GetInsertBlock()->getParent() == PredefFunctions::main(*llvm_module))
		var = new GlobalVariable(*llvm_module, llvm_type, false, GlobalValue::ExternalLinkage, nullptr, name + ".ptr");
	else
		var = ir_builder->CreateAlloca(llvm_type, nullptr, name + ".ptr");
	assert(!m_declarations.contains(declaration.get_declaration_id()));
	m_declarations[declaration.get_declaration_id()] = var;
}

void CodeGen::visit(const Function& function) {
	auto [name, type] = DeclarationDumpster::the().retrieve(function.get_function_declaration_id());
	const FunctionType& function_type = static_cast<const FunctionType&>(type->get_declared_type());
	Type* return_type = Utils::get_llvm_type_for(llvm_module->getContext(), *function_type.get_returned_type());
	std::vector<Type*> params;
	for(const RefPtr<AppliedType>& param_type : function_type.get_parameter()) {
		Type* llvm_param_type = Utils::get_llvm_type_for(llvm_module->getContext(), param_type->get_declared_type());
		params.push_back(Utils::get_ptr_type(llvm_param_type));
	}
	llvm::FunctionType* llvm_function_type = llvm::FunctionType::get(return_type, params, false);
	llvm::Function* llvm_function =
		llvm::Function::Create(llvm_function_type, llvm::Function::PrivateLinkage, name, *llvm_module);
	assert(!m_declarations.contains(function.get_function_declaration_id()));
	m_declarations[function.get_function_declaration_id()] = llvm_function;

	for(unsigned i = 0; i < function.get_parameters().size(); ++i) {
		Argument* arg = llvm_function->getArg(i);
		declid_t id = function.get_parameters().at(i);
		assert(!m_declarations.contains(id));
		m_declarations[id] = arg;
		std::string_view name = DeclarationDumpster::the().retrieve(id).name;
		arg->setName(name);
	}

	BasicBlock* entry = BasicBlock::Create(llvm_module->getContext());
	llvm_function->getBasicBlockList().push_back(entry);
	Utils::generate_on_basic_block(
		*ir_builder, entry, [&]() { function.get_implementation().accept(*this); }, true);
}

void CodeGen::visit(const Return& return_statement) {
	Value* return_value = visit_with_return(return_statement.get_expression());
	ir_builder->CreateRet(return_value);
}

void CodeGen::visit(const Block& block) {
	for(const RefPtr<Statement>& statement : block.get_body())
		statement->accept(*this);
}

void CodeGen::visit(const IntLiteral& literal) {
	unsigned width = static_cast<const IntType&>(literal.get_type().get_declared_type()).get_width();
	ConstantInt* literal_value = Utils::get_integer_constant(*llvm_module, literal.get_value(), width);
	return_from_visit(literal_value);
}

void CodeGen::visit(const Assignment& assignment) {
	Value* new_value = visit_with_return(assignment.get_rhs());
	Value* variable = m_declarations.at(assignment.get_lhs());
	ir_builder->CreateStore(new_value, variable);
	return_from_visit(new_value);
}

void CodeGen::visit(const BinaryExpression& binary_expression) {
	Value* lhs = visit_with_return(binary_expression.get_lhs());
	Value* rhs = visit_with_return(binary_expression.get_rhs());
	Value* result = nullptr;
	switch(binary_expression.get_operator().get_type()) {
		case TokenType::PLUS: result = ir_builder->CreateAdd(lhs, rhs); break;
		case TokenType::MINUS: result = ir_builder->CreateSub(lhs, rhs); break;
		case TokenType::STAR: result = ir_builder->CreateMul(lhs, rhs); break;
		case TokenType::SLASH: result = ir_builder->CreateSDiv(lhs, rhs); break;
		default: assert_not_reached();
	}
	return_from_visit(result);
}

void CodeGen::visit(const Call& call) {
	llvm::Function* llvm_function = cast<llvm::Function>(m_declarations.at(call.get_function_declaration_id()));
	std::vector<Value*> arguments;
	for(const RefPtr<Expression>& arg : call.get_arguments())
		arguments.push_back(visit_with_return(*arg));
	Value* result = ir_builder->CreateCall(llvm_function, arguments);
	return_from_visit(result);
}

void CodeGen::visit(const VarQuery& var_query) {
	Value* variable = m_declarations.at(var_query.get_declaration_id());
	Type* expected_type = Utils::get_llvm_type_for(llvm_module->getContext(), var_query.get_type().get_declared_type());
	std::string_view name = DeclarationDumpster::the().retrieve(var_query.get_declaration_id()).name;
	Value* loaded_variable = ir_builder->CreateLoad(expected_type, variable, name);
	return_from_visit(loaded_variable);
}

}
