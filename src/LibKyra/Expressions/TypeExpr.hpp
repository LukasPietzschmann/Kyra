#pragma once

#include <memory>
#include <string>
#include <vector>

#include "Expression.hpp"

namespace Kyra {
struct Position;
class TypeExpr : public Expression {
public:
	TypeExpr(const Position& position, std::string&& name);
	TypeExpr(const Position& position,
			std::vector<std::shared_ptr<TypeExpr>>&& parameter_types,
			std::shared_ptr<TypeExpr> return_type);
	~TypeExpr() override = default;

	void accept(ExpressionVisitor& visitor) override;
	const std::string& get_name() const;
	const std::vector<std::shared_ptr<TypeExpr>>& get_parameter_types() const;
	std::shared_ptr<TypeExpr> get_return_type() const;
	bool is_function() const;

private:
	struct TypesForFunctions {
		TypesForFunctions(std::vector<std::shared_ptr<TypeExpr>> parameter_types,
				std::shared_ptr<TypeExpr> return_type);
		std::vector<std::shared_ptr<TypeExpr>> parameter_types;
		std::shared_ptr<TypeExpr> return_type;
	};
	std::string m_name;
	std::shared_ptr<TypesForFunctions> m_types_for_functions;
};
}