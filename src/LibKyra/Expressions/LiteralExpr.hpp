#pragma once

#include <string>
#include <variant>

#include "Expression.hpp"

namespace Kyra {
struct Position;
class LiteralExpr : public Expression {
public:
	enum Type { t_int, t_string, t_bool, t_nothing };

	LiteralExpr(const Position& position, int int_value);
	LiteralExpr(const Position& position, std::string&& string_value);
	LiteralExpr(const Position& position, bool bool_value);
	explicit LiteralExpr(const Position& position);

	void accept(ExpressionVisitor& visitor) override;
	Type get_type() const;
	int get_int_value() const;
	const std::string& get_string_value() const;
	bool get_bool_value() const;

private:
	Type m_type;
	std::variant<int, std::string, bool> m_literal;
};
}