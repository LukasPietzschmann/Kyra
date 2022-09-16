#pragma once

#include <map>
#include <optional>
#include <string_view>
#include <utility>
#include <vector>

#include "AST.hpp"
#include "Aliases.hpp"
#include "Error.hpp"
#include "TAST.hpp"
#include "Type.hpp"

namespace Kyra {

class TypeChecker : public Untyped::ASTVisitor {
private:
	struct VisitResult {
		VisitResult(RefPtr<AppliedType> type, RefPtr<Typed::Expression> expression) :
			type(std::move(type)), expression(std::move(expression)) {}
		VisitResult() : VisitResult(nullptr, nullptr) {}

		RefPtr<AppliedType> type;
		RefPtr<Typed::Expression> expression;
	};

	VISIT_RETURN_TYPE(VisitResult)

public:
	struct Context {
		RefPtr<FunctionType> enclosing_function{nullptr};
		bool had_return{false};
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

	ErrorOr<std::vector<RefPtr<Typed::Statement>>> check_statements(
		const std::vector<RefPtr<Untyped::Statement>>& statements);

	void visit(const Untyped::ExpressionStatement& expresion_statement) override;
	void visit(const Untyped::Declaration& declaration) override;
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
	std::vector<RefPtr<Typed::Statement>> m_typed_statements;
	RefPtr<TypeScope> m_current_scope{mk_ref<TypeScope>()};
	Context m_context;

	template <typename Callback>
	void execute_on_scope(RefPtr<TypeScope> scope, Callback callback);
	template <typename Callback>
	RefPtr<TypeScope> execute_on_new_scope(Callback callback);
};
}
