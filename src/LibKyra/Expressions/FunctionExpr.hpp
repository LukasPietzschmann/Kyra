#pragma once

#include <utility>
#include <vector>

#include "../Statements/Statement.hpp"
#include "../Token.hpp"
#include "Expression.hpp"

namespace Kyra {
class FunctionExpr : public Expression {
public:
	struct Parameter {
		Parameter(Token name, std::string type);
		Token name;
		std::string type;
	};

	FunctionExpr(const Position& position,
			std::vector<Parameter> parameters,
			const Expression::Ptr& returnType,
			const Statement::Ptr& impl);
	~FunctionExpr() override = default;

	void accept(ExpressionVisitor& visitor) override;
	const std::vector<Parameter>& getParameters() const;
	Expression::Ptr getReturnType() const;
	Statement::Ptr getImplementation() const;

private:
	std::vector<Parameter> m_parameters;
	Expression::Ptr m_returnType;
	Statement::Ptr m_implementation;
};
}