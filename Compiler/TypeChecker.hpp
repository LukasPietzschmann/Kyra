#pragma once

#include <map>
#include <optional>
#include <string_view>

#include "AST.hpp"
#include "Aliases.hpp"
#include "Error.hpp"
#include "Type.hpp"

namespace Kyra {

class TypeChecker : public ASTVisitor {
	VISIT_RETURN_TYPE(RefPtr<AppliedType>)
public:
	struct Context {
		RefPtr<FunctionType> enclosing_function{nullptr};
	};

	static TypeChecker& the() {
		static TypeChecker instance;
		return instance;
	}

	TypeChecker() = default;
	TypeChecker(const TypeChecker&) = delete;
	TypeChecker(TypeChecker&&) noexcept = default;

	TypeChecker& operator=(const TypeChecker&) = delete;
	TypeChecker& operator=(TypeChecker&&) noexcept = default;

	ErrorOr<void> check_statements(const std::vector<RefPtr<Statement>>& statements);

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
	RefPtr<TypeScope> m_current_scope{mk_ref<TypeScope>()};
	Context m_context;

	template <typename Callback>
	void execute_on_scope(RefPtr<TypeScope> scope, Callback callback);
	template <typename Callback>
	RefPtr<TypeScope> execute_on_new_scope(Callback callback);
};
}
