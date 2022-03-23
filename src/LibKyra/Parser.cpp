#include "Parser.hpp"

#include <algorithm>
#include <memory>
#include <string>

#include "Exceptions.hpp"
#include "Expressions/AccessExpr.hpp"
#include "Expressions/AssignmentExpr.hpp"
#include "Expressions/BinaryExpr.hpp"
#include "Expressions/CallExpr.hpp"
#include "Expressions/GroupExpr.hpp"
#include "Expressions/InstantiationExpr.hpp"
#include "Expressions/LambdaFunctionExpr.hpp"
#include "Expressions/LiteralExpr.hpp"
#include "Expressions/TypeExpr.hpp"
#include "Expressions/UnaryExpr.hpp"
#include "Expressions/VariableExpr.hpp"
#include "Position.hpp"
#include "Statements/BlockStmt.hpp"
#include "Statements/ClassDeclarationStmt.hpp"
#include "Statements/ExpressionStmt.hpp"
#include "Statements/FunDeclarationStmt.hpp"
#include "Statements/IfStmt.hpp"
#include "Statements/PrintStmt.hpp"
#include "Statements/ReturnStmt.hpp"
#include "Statements/VarDeclarationStmt.hpp"
#include "Statements/WhileStmt.hpp"
#include "Values/Value.hpp"

namespace Kyra {
const Result<std::vector<Statement::Ptr>>& Parser::parse() {
	m_result.reset();
	m_result.construct_value();
	try {
		while(!is_at_end())
			m_result.assert_get_value().emplace_back(declaration());
	} catch(const ParserException& e) { m_result.insert_error({e.what(), e.get_position()}); }
	return m_result;
}

Statement::Ptr Parser::declaration() {
	if(!match({TokenType::VAR, TokenType::VAL, TokenType::CLASS, TokenType::FUN}))
		return statement();

	if(match(TokenType::CLASS))
		return class_declaration();

	if(match({TokenType::VAR, TokenType::VAL}))
		return var_declaration();

	return function_declaration();
}

Statement::Ptr Parser::var_declaration() {
	const Token& val_var_kw = consume({TokenType::VAL, TokenType::VAR});
	const bool is_mutable = previous().get_type() == TokenType::VAR;

	Token identifier = consume(TokenType::NAME);

	std::shared_ptr<TypeExpr> expected_type{};
	if(match_and_advance(TokenType::COLON))
		expected_type = type_indicator();

	Expression::Ptr init = nullptr;
	if(match_and_advance(TokenType::EQUAL))
		init = assignment();
	const Token& semicolon = consume(TokenType::SEMICOLON);

	if(!is_mutable && init == nullptr)
		throw ParserException(Position(val_var_kw.get_position(), semicolon.get_position()),
				"Immutable Variable " + identifier.get_value().as_string() + " needs to be initialized");

	const Position position(val_var_kw.get_position(), semicolon.get_position());
	return Statement::make_ptr<VarDeclarationStmt>(position,
			identifier,
			std::move(init),
			std::move(expected_type),
			is_mutable);
}

Statement::Ptr Parser::class_declaration() {
	const Token& class_kw = consume(TokenType::CLASS);
	const Token& name = consume(TokenType::NAME);

	std::vector<ClassDeclarationStmt::ConstructorParameter> constructor_parameter;
	consume(TokenType::LEFT_PAREN);
	if(!match(TokenType::RIGHT_PAREN)) {
		do {
			match_and_advance({TokenType::VAR, TokenType::VAL});
			const bool is_mutable = previous().get_type() == TokenType::VAR;
			const Token& param_name = consume(TokenType::NAME);
			consume(TokenType::COLON);
			constructor_parameter.emplace_back(param_name, is_mutable, type_indicator());
		} while(match_and_advance(TokenType::COMMA));
	}
	consume(TokenType::RIGHT_PAREN);
	consume(TokenType::LEFT_CURLY);

	std::vector<std::shared_ptr<VarDeclarationStmt>> declarations;
	while(!is_at_end() && !match(TokenType::RIGHT_CURLY))
		declarations.push_back(std::dynamic_pointer_cast<VarDeclarationStmt>(var_declaration()));

	const Token& closing_curly = consume(TokenType::RIGHT_CURLY);
	const Position position(class_kw.get_position(), closing_curly.get_position());
	return Statement::make_ptr<ClassDeclarationStmt>(position,
			name,
			std::move(constructor_parameter),
			std::move(declarations));
}

Statement::Ptr Parser::function_declaration() {
	const Token& fun_kw = consume(TokenType::FUN);
	const Token& name = consume(TokenType::NAME);

	consume(TokenType::LEFT_PAREN);
	std::vector<LambdaFunctionExpr::Parameter> parameters;
	if(!match(TokenType::RIGHT_PAREN)) {
		do {
			const Token& param_name = consume(TokenType::NAME);
			consume(TokenType::COLON);
			parameters.emplace_back(param_name, type_indicator());
		} while(match_and_advance(TokenType::COMMA));
	}
	consume(TokenType::RIGHT_PAREN);
	consume(TokenType::ARROW);

	std::shared_ptr<TypeExpr> return_type = type_indicator();
	Statement::Ptr implementation = block();

	const Position position(fun_kw.get_position(), implementation->get_position());
	auto function = std::make_shared<LambdaFunctionExpr>(position,
			std::move(parameters),
			std::move(return_type),
			std::move(implementation));
	return Statement::make_ptr<FunDeclarationStmt>(position, name, function);
}

Statement::Ptr Parser::statement() {
	if(match(TokenType::LEFT_CURLY))
		return block();
	if(match(TokenType::PRINT))
		return print();
	if(match(TokenType::RETURN))
		return ret();
	if(match(TokenType::WHILE))
		return while_loop();
	if(match(TokenType::IF))
		return if_stmt();
	return expression();
}

Statement::Ptr Parser::block() {
	const Token& left_curly = consume(TokenType::LEFT_CURLY);

	std::vector<Statement::Ptr> statements;
	while(!match(TokenType::RIGHT_CURLY) && !is_at_end())
		statements.push_back(declaration());

	const Token& right_curly = consume(TokenType::RIGHT_CURLY);
	return Statement::make_ptr<BlockStmt>(Position(left_curly.get_position(), right_curly.get_position()),
			std::move(statements));
}

Statement::Ptr Parser::print() {
	const Token& print_kw = consume(TokenType::PRINT);
	Expression::Ptr expr = assignment();
	const Token& semicolon = consume(TokenType::SEMICOLON);
	return Statement::make_ptr<PrintStmt>(Position(print_kw.get_position(), semicolon.get_position()), std::move(expr));
}

Statement::Ptr Parser::ret() {
	const Token& return_kw = consume(TokenType::RETURN);
	Expression::Ptr expr = assignment();
	const Token& semicolon = consume(TokenType::SEMICOLON);
	return Statement::make_ptr<ReturnStmt>(Position(return_kw.get_position(), semicolon.get_position()),
			std::move(expr));
}

Statement::Ptr Parser::while_loop() {
	const Token& while_kw = consume(TokenType::WHILE);
	Expression::Ptr condition = group();
	Statement::Ptr stmt = statement();
	const Position& stmt_pos = stmt->get_position();
	return Statement::make_ptr<WhileStmt>(Position(while_kw.get_position(), stmt_pos),
			std::move(condition),
			std::move(stmt));
}

Statement::Ptr Parser::if_stmt() {
	const Token& if_kw = consume(TokenType::IF);
	Expression::Ptr condition = group();
	Statement::Ptr then = statement();
	Statement::Ptr else_if{};
	Position end_pos(then->get_position());
	if(match_and_advance(TokenType::ELSE)) {
		else_if = statement();
		end_pos = else_if->get_position();
	}
	return Statement::make_ptr<IfStmt>(Position(if_kw.get_position(), end_pos),
			std::move(condition),
			std::move(then),
			std::move(else_if));
}

Statement::Ptr Parser::expression() {
	Expression::Ptr expr = assignment();
	const Token& semicolon = consume(TokenType::SEMICOLON);
	return Statement::make_ptr<ExpressionStmt>(Position(expr->get_position(), semicolon.get_position()),
			std::move(expr));
}

Expression::Ptr Parser::assignment() {
	Expression::Ptr expr = equality();

	if(match_and_advance(TokenType::EQUAL)) {
		auto access_expression = std::dynamic_pointer_cast<AccessExpr>(expr);
		auto variable_expression = std::dynamic_pointer_cast<VariableExpr>(expr);
		if(access_expression) {
			Expression::Ptr new_value = assignment();
			const Position position(expr->get_position(), new_value->get_position());
			expr = Expression::make_ptr<AssignmentExpr>(position,
					access_expression->get_owner(),
					access_expression->get_name(),
					new_value);
		} else if(variable_expression) {
			Expression::Ptr new_value = assignment();
			const Position position(expr->get_position(), new_value->get_position());
			expr = Expression::make_ptr<AssignmentExpr>(position, nullptr, variable_expression->get_name(), new_value);
		}
	}
	return expr;
}

Expression::Ptr Parser::equality() {
	Expression::Ptr expr = comparison();

	while(true) {
		if(!match_and_advance({TokenType::BANG_EQUAL, TokenType::EQUAL_EQUAL}))
			break;
		const Token& oper = previous();
		Expression::Ptr rhs = comparison();
		const Position& expr_pos = expr->get_position();
		const Position& rhs_pos = rhs->get_position();
		expr = Expression::make_ptr<BinaryExpr>(Position(expr_pos, rhs_pos), std::move(expr), oper, std::move(rhs));
	}
	return expr;
}

Expression::Ptr Parser::comparison() {
	Expression::Ptr expr = term();

	while(true) {
		if(!(match_and_advance({TokenType::GREATER, TokenType::GREATER_EQUAL, TokenType::LESS, TokenType::LESS_EQUAL})))
			break;
		const Token& oper = previous();
		Expression::Ptr rhs = term();
		const Position& expr_pos = expr->get_position();
		const Position& rhs_pos = rhs->get_position();
		expr = Expression::make_ptr<BinaryExpr>(Position(expr_pos, rhs_pos), std::move(expr), oper, std::move(rhs));
	}
	return expr;
}

Expression::Ptr Parser::term() {
	Expression::Ptr expr = factor();

	while(true) {
		if(!(match_and_advance({TokenType::MINUS, TokenType::PLUS})))
			break;
		const Token& oper = previous();
		Expression::Ptr rhs = factor();
		const Position& expr_pos = expr->get_position();
		const Position& rhs_pos = rhs->get_position();
		expr = Expression::make_ptr<BinaryExpr>(Position(expr_pos, rhs_pos), std::move(expr), oper, std::move(rhs));
	}
	return expr;
}

Expression::Ptr Parser::factor() {
	Expression::Ptr expr = unary();

	while(true) {
		if(!(match_and_advance({TokenType::SLASH, TokenType::STAR})))
			break;
		const Token& oper = previous();
		Expression::Ptr rhs = unary();
		const Position& expr_pos = expr->get_position();
		const Position& rhs_pos = rhs->get_position();
		expr = Expression::make_ptr<BinaryExpr>(Position(expr_pos, rhs_pos), std::move(expr), oper, std::move(rhs));
	}
	return expr;
}

Expression::Ptr Parser::unary() {
	if(!(match_and_advance({TokenType::BANG, TokenType::MINUS})))
		return call();

	const Token& oper = previous();
	Expression::Ptr rhs = unary();
	const Position& rhs_pos = rhs->get_position();
	return Expression::make_ptr<UnaryExpr>(Position(oper.get_position(), rhs_pos), oper, std::move(rhs));
}

Expression::Ptr Parser::call() {
	Expression::Ptr expr = primary();

	while(true) {
		if(match_and_advance(TokenType::DOT)) {
			const Token& name = consume(TokenType::NAME);
			const Position& expr_pos = expr->get_position();
			expr = Expression::make_ptr<AccessExpr>(Position(expr_pos, name.get_position()), std::move(expr), name);
			continue;
		}
		if(match_and_advance(TokenType::LEFT_PAREN)) {
			std::vector<Expression::Ptr> arguments;
			if(!match(TokenType::RIGHT_PAREN)) {
				do {
					arguments.push_back(assignment());
				} while(match_and_advance(TokenType::COMMA));
			}
			const Token& right_paren = consume(TokenType::RIGHT_PAREN);
			const Position& expr_pos = expr->get_position();
			expr = Expression::make_ptr<CallExpr>(Position(expr_pos, right_paren.get_position()),
					std::move(expr),
					std::move(arguments));
			continue;
		}
		break;
	}
	return expr;
}

Expression::Ptr Parser::primary() {
	if(match(TokenType::LEFT_PAREN))
		return group();
	if(match(TokenType::NOTHING)) {
		const Position& position = consume(TokenType::NOTHING).get_position();
		return Expression::make_ptr<LiteralExpr>(position);
	}
	if(match(TokenType::NUMBER)) {
		const Position& position = consume(TokenType::NUMBER).get_position();
		return Expression::make_ptr<LiteralExpr>(position, previous().get_value().as_int());
	}
	if(match(TokenType::STRING)) {
		const Position& position = consume(TokenType::STRING).get_position();
		return Expression::make_ptr<LiteralExpr>(position, previous().get_value().as_string());
	}
	if(match(TokenType::TRUE)) {
		const Position& position = consume(TokenType::TRUE).get_position();
		return Expression::make_ptr<LiteralExpr>(position, true);
	}
	if(match(TokenType::FALSE)) {
		const Position& position = consume(TokenType::FALSE).get_position();
		return Expression::make_ptr<LiteralExpr>(position, false);
	}
	if(match(TokenType::NAME)) {
		const Position& position = consume(TokenType::NAME).get_position();
		return Expression::make_ptr<VariableExpr>(position, previous());
	}
	if(match(TokenType::FUN))
		return lambda_function();
	if(match(TokenType::INSTANTIATE))
		return instantiation();

	if(previous().get_type() == TokenType::END_OF_FILE)
		throw ParserException(peek().get_position(), "Expected expression before " + peek().get_lexeme());
	else
		throw ParserException(previous().get_position(), "Expected expression after " + previous().get_lexeme(), true);
}

Expression::Ptr Parser::lambda_function() {
	const Token& fun_kw = consume(TokenType::FUN);

	std::vector<LambdaFunctionExpr::Parameter> parameters;
	consume(TokenType::LEFT_PAREN);
	if(!match(TokenType::RIGHT_PAREN)) {
		do {
			const Token& name = consume(TokenType::NAME);
			consume(TokenType::COLON);
			parameters.emplace_back(name, type_indicator());
		} while(match_and_advance(TokenType::COMMA));
	}
	consume(TokenType::RIGHT_PAREN);
	consume(TokenType::ARROW);

	std::shared_ptr<TypeExpr> return_type = type_indicator();
	Statement::Ptr implementation = block();

	const Position position(fun_kw.get_position(), implementation->get_position());
	return Expression::make_ptr<LambdaFunctionExpr>(position,
			std::move(parameters),
			std::move(return_type),
			std::move(implementation));
}

Expression::Ptr Parser::instantiation() {
	const Token& instantiate_kw = consume(TokenType::INSTANTIATE);
	const Token& name = consume(TokenType::NAME);

	std::vector<Expression::Ptr> parameters;
	consume(TokenType::LEFT_PAREN);
	if(!match(TokenType::RIGHT_PAREN)) {
		do {
			parameters.emplace_back(assignment());
		} while(match_and_advance(TokenType::COMMA));
	}
	const Token& right_paren = consume(TokenType::RIGHT_PAREN);

	const Position position(instantiate_kw.get_position(), right_paren.get_position());
	return Expression::make_ptr<InstantiationExpr>(position, name, std::move(parameters));
}

Expression::Ptr Parser::group() {
	const Token& left_paren = consume(TokenType::LEFT_PAREN);
	Expression::Ptr expr = assignment();
	const Token& right_paren = consume(TokenType::RIGHT_PAREN);
	return Expression::make_ptr<GroupExpr>(Position(left_paren.get_position(), right_paren.get_position()),
			std::move(expr));
}

std::shared_ptr<TypeExpr> Parser::type_indicator() {
	const Token& type_name = consume(TokenType::NAME);
	if(type_name.get_value().as_string() == Value::NativeTypes::Function) {
		consume(TokenType::LEFT_PAREN);
		std::vector<std::shared_ptr<TypeExpr>> param_types;
		if(!match(TokenType::RIGHT_PAREN)) {
			do {
				param_types.push_back(type_indicator());
			} while(match_and_advance(TokenType::COMMA));
		}
		consume(TokenType::RIGHT_PAREN);
		consume(TokenType::ARROW);
		std::shared_ptr<TypeExpr> return_type = type_indicator();

		const Position position(type_name.get_position(), return_type->get_position());
		return std::make_shared<TypeExpr>(position, std::move(param_types), std::move(return_type));
	}
	return std::make_shared<TypeExpr>(type_name.get_position(), type_name.get_value().as_string());
}

Token Parser::advance() { return m_tokens[m_current++]; }

Token Parser::consume(TokenType expected) { return consume({expected}); }

Token Parser::consume(std::initializer_list<TokenType> expected) {
	std::string name;
	for(const auto& e : expected) {
		if(!is_at_end() && peek().get_type() == e)
			return advance();
		name += TokenTypeName::get_for(e) + " ";
	}

	const bool unfinished = peek().get_type() == TokenType::END_OF_FILE;
	throw ParserException(peek().get_position(),
			"Expected " + name + "but got " + TokenTypeName::get_for(peek().get_type()),
			unfinished);
}

Token Parser::peek() const { return m_tokens[m_current]; }

bool Parser::match(TokenType expected) const { return match({expected}); }
bool Parser::match_and_advance(TokenType expected) { return match_and_advance({expected}); }

bool Parser::match(std::initializer_list<TokenType> expected) const {
	if(is_at_end())
		return false;
	return std::any_of(expected.begin(), expected.end(), [this](TokenType type) { return type == peek().get_type(); });
}

bool Parser::match_and_advance(std::initializer_list<TokenType> expected) {
	const bool res = match(expected);
	if(res)
		advance();
	return res;
}

Token Parser::previous() const { return m_tokens[m_current - 1]; }

bool Parser::is_at_end() const { return peek().get_type() == TokenType::END_OF_FILE; }
}