#pragma once

#include <string>
#include <vector>

#include "../Statements/Statement.hpp"
#include "../Token.hpp"
#include "Expression.hpp"

namespace Kyra {
struct Position;
class TypeExpr;
class FunctionExpr : public Expression {
public:
	struct Parameter {
		Parameter(Token name, std::shared_ptr<TypeExpr> type);
		Token name;
		std::shared_ptr<TypeExpr> type;
	};

	FunctionExpr(const Position& position,
			std::vector<Parameter>&& parameters,
			std::shared_ptr<TypeExpr> return_type,
			Statement::Ptr impl);
	~FunctionExpr() override = default;

	void accept(ExpressionVisitor& visitor) override;
	const std::vector<Parameter>& get_parameters() const;
	std::shared_ptr<TypeExpr> get_return_type() const;
	Statement::Ptr get_implementation() const;

private:
	std::vector<Parameter> m_parameters;
	std::shared_ptr<TypeExpr> m_return_type;
	Statement::Ptr m_implementation;
};
}