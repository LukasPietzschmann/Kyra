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
	TypeExpr(const Position& position, std::vector<Expression::Ptr> parameterTypes, Expression::Ptr returnType);
	~TypeExpr() override = default;

	void accept(ExpressionVisitor& visitor) override;
	const std::string& getName() const;
	const std::vector<Expression::Ptr>& getParameterTypes() const;
	const Expression::Ptr& getReturnType() const;
	bool isFunction() const;

private:
	struct TypesForFunctions {
		TypesForFunctions(std::vector<Expression::Ptr> parameterTypes, Expression::Ptr returnType);
		std::vector<Expression::Ptr> parameterTypes;
		Expression::Ptr returnType;
	};
	std::string m_name;
	std::shared_ptr<TypesForFunctions> m_typesForFunctions;
};
}