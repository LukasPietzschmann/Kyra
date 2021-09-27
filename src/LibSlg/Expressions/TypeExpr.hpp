#pragma once

#include <string>

#include "Expression.hpp"

namespace LibSlg {
class TypeExpr : public Expression {
public:
	TypeExpr(const Position& position, std::string name) :
		Expression(position), m_name(std::move(name)), m_typesForFunctions(nullptr) {}
	TypeExpr(const Position& position, std::vector<Expression::Ptr> parameterTypes, Expression::Ptr returnType) :
		Expression(position), m_name("Function"),
		m_typesForFunctions(std::make_shared<TypesForFunctions>(parameterTypes, returnType)) {}
	~TypeExpr() override = default;
	void accept(ExpressionVisitor& visitor) override { visitor.visitTypeExpr(*this); }

	const std::string& getName() const { return m_name; }

	const std::vector<Expression::Ptr>& getParameterTypes() const {
		assert(m_typesForFunctions != nullptr);
		return m_typesForFunctions->parameterTypes;
	};

	const Expression::Ptr& getReturnType() const {
		assert(m_typesForFunctions != nullptr);
		return m_typesForFunctions->returnType;
	}

	bool isFunction() const { return m_typesForFunctions != nullptr; }

private:
	struct TypesForFunctions {
		TypesForFunctions(std::vector<Expression::Ptr> parameterTypes, Expression::Ptr returnType) :
			parameterTypes(std::move(parameterTypes)), returnType(std::move(returnType)) {}
		std::vector<Expression::Ptr> parameterTypes;
		Expression::Ptr returnType;
	};
	std::string m_name;
	std::shared_ptr<TypesForFunctions> m_typesForFunctions;
};
}