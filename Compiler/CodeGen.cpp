#include "CodeGen.hpp"

namespace Kyra {

void CodeGen::gen_code(const std::vector<RefPtr<Statement>>& statements) {
	for(const RefPtr<Statement>& statement : statements)
		statement->accept(*this);
}

void CodeGen::visit(const ExpressionStatement& expresion_statement) {}

void CodeGen::visit(const Declaration& declaration) {}

void CodeGen::visit(const Function& function) {}

void CodeGen::visit(const Return& return_statement) {}

void CodeGen::visit(const Block& block) {}

void CodeGen::visit(const IntLiteral& literal) {}

void CodeGen::visit(const Assignment& assignment) {}

void CodeGen::visit(const BinaryExpression& binary_expression) {}

void CodeGen::visit(const TypeIndicator& type) {}

void CodeGen::visit(const Call& call) {}

void CodeGen::visit(const Group& group) {}

void CodeGen::visit(const VarQuery& var_query) {}
}
