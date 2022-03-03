#pragma once

#include <memory>
#include <string>
#include <vector>

#include "Expression.hpp"

namespace Kyra {
struct Position;
class TypeExpr : public Expression {
public:
	TypeExpr(const Position& position, std::string name);
	TypeExpr(const Position& position, std::vector<Expression::Ptr> parameter_types, Expression::Ptr return_type);
	~TypeExpr() override = default;

	void accept(ExpressionVisitor& visitor) override;
	const std::string& get_name() const;
	const std::vector<Expression::Ptr>& get_parameter_types() const;
	const Expression::Ptr& get_return_type() const;
	bool is_function() const;

private:
	struct TypesForFunctions {
		TypesForFunctions(std::vector<Expression::Ptr> parameter_types, Expression::Ptr return_type);
		std::vector<Expression::Ptr> parameter_types;
		Expression::Ptr return_type;
	};
	std::string m_name;
	std::shared_ptr<TypesForFunctions> m_types_for_functions;
};
}