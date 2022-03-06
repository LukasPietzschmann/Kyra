#pragma once

#include <string>
#include <vector>

#include "../Statements/Statement.hpp"
#include "../Token.hpp"
#include "Expression.hpp"

namespace Kyra {
struct Position;
class FunctionExpr : public Expression {
public:
	struct Parameter {
		Parameter(Token name, std::string type);
		Token name;
		std::string type;
	};

	FunctionExpr(const Position& position,
			std::vector<Parameter>&& parameters,
			Expression::Ptr return_type,
			Statement::Ptr impl);
	~FunctionExpr() override = default;

	void accept(ExpressionVisitor& visitor) override;
	const std::vector<Parameter>& get_parameters() const;
	Expression::Ptr get_return_type() const;
	Statement::Ptr get_implementation() const;

private:
	std::vector<Parameter> m_parameters;
	Expression::Ptr m_return_type;
	Statement::Ptr m_implementation;
};
}