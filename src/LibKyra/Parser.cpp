#include "Parser.hpp"

#include <algorithm>
#include <iostream>
#include <memory>
#include <string>

#include "Exceptions.hpp"
#include "Expressions/AccessExpr.hpp"
#include "Expressions/AssignmentExpr.hpp"
#include "Expressions/BinaryExpr.hpp"
#include "Expressions/CallExpr.hpp"
#include "Expressions/FunctionExpr.hpp"
#include "Expressions/GroupExpr.hpp"
#include "Expressions/InstantiationExpr.hpp"
#include "Expressions/LiteralExpr.hpp"
#include "Expressions/TypeExpr.hpp"
#include "Expressions/UnaryExpr.hpp"
#include "Expressions/VariableExpr.hpp"
#include "Position.hpp"
#include "Statements/BlockStmt.hpp"
#include "Statements/ClassDeclarationStmt.hpp"
#include "Statements/DeclarationStmt.hpp"
#include "Statements/ExpressionStmt.hpp"
#include "Statements/PrintStmt.hpp"
#include "Statements/ReturnStmt.hpp"
#include "Values/Bool.hpp"
#include "Values/Nothing.hpp"
#include "Values/Number.hpp"
#include "Values/String.hpp"
#include "Values/Value.hpp"

namespace Kyra {
std::vector<Statement::Ptr> Parser::parse() {
	while(!isAtEnd())
		m_statements.push_back(declaration());
	return m_statements;
}

Statement::Ptr Parser::declaration() {
	if(!match({TokenType::VAR, TokenType::VAL, TokenType::CLASS}))
		return statement();

	if(match(TokenType::CLASS))
		return classDeclaration();

	return varDeclaration();
}

Statement::Ptr Parser::varDeclaration() {
	const Token& valVarKW = consume({TokenType::VAL, TokenType::VAR});
	bool isMutable = previous().getType() == TokenType::VAR;

	Token identifier = consume(TokenType::NAME);

	Expression::Ptr expectedType{};
	if(matchAndAdvance(TokenType::COLON))
		expectedType = typeIndicator();

	Expression::Ptr init = nullptr;
	if(matchAndAdvance(TokenType::EQUAL))
		init = assignment();
	const Token& semicolon = consume(TokenType::SEMICOLON);

	if(!isMutable && init == nullptr)
		throw ParserException(Position(valVarKW.getPosition(), semicolon.getPosition()),
				"Immutable Variable " + identifier.getValue().asString() + " needs to be initialized");

	Position position(valVarKW.getPosition(), semicolon.getPosition());
	return Statement::makePtr<DeclarationStmt>(position, identifier, init, expectedType, isMutable);
}

Statement::Ptr Parser::classDeclaration() {
	const Token& classKW = consume(TokenType::CLASS);
	Token name = consume(TokenType::NAME);

	std::vector<ClassDeclarationStmt::ConstructorParameter> constructorParameter;
	consume(TokenType::LEFT_PAREN);
	if(!match(TokenType::RIGHT_PAREN)) {
		do {
			matchAndAdvance({TokenType::VAR, TokenType::VAL});
			bool isMutable = previous().getType() == TokenType::VAR;
			Token paramName = consume(TokenType::NAME);
			consume(TokenType::COLON);
			std::string type = consume(TokenType::NAME).getValue().asString();
			constructorParameter.emplace_back(paramName, isMutable, type);
		} while(matchAndAdvance(TokenType::COMMA));
	}
	consume(TokenType::RIGHT_PAREN);
	consume(TokenType::LEFT_CURLY);

	std::vector<std::shared_ptr<DeclarationStmt>> declarations;
	while(!isAtEnd() && !match(TokenType::RIGHT_CURLY))
		declarations.push_back(std::dynamic_pointer_cast<DeclarationStmt>(varDeclaration()));

	const Token& closingCurly = consume(TokenType::RIGHT_CURLY);
	Position position(classKW.getPosition(), closingCurly.getPosition());
	return Statement::makePtr<ClassDeclarationStmt>(position, name, constructorParameter, declarations);
}

Statement::Ptr Parser::statement() {
	if(match(TokenType::LEFT_CURLY))
		return block();
	if(match(TokenType::PRINT))
		return print();
	if(match(TokenType::RETURN))
		return ret();
	return expression();
}

Statement::Ptr Parser::block() {
	const Token& leftCurly = consume(TokenType::LEFT_CURLY);

	std::vector<Statement::Ptr> statements;
	while(!match(TokenType::RIGHT_CURLY) && !isAtEnd())
		statements.push_back(declaration());

	const Token& rightCurly = consume(TokenType::RIGHT_CURLY);

	return Statement::makePtr<BlockStmt>(Position(leftCurly.getPosition(), rightCurly.getPosition()), statements);
}

Statement::Ptr Parser::print() {
	const Token& printKW = consume(TokenType::PRINT);
	Expression::Ptr expr = assignment();
	const Token& semicolon = consume(TokenType::SEMICOLON);

	return Statement::makePtr<PrintStmt>(Position(printKW.getPosition(), semicolon.getPosition()), expr);
}

Statement::Ptr Parser::ret() {
	const Token& returnKW = consume(TokenType::RETURN);
	Expression::Ptr expr = assignment();
	const Token& semicolon = consume(TokenType::SEMICOLON);

	return Statement::makePtr<ReturnStmt>(Position(returnKW.getPosition(), semicolon.getPosition()), expr);
}

Statement::Ptr Parser::expression() {
	Expression::Ptr expr = assignment();
	const Token& semicolon = consume(TokenType::SEMICOLON);

	return Statement::makePtr<ExpressionStmt>(Position(expr->getPosition(), semicolon.getPosition()), expr);
}

Expression::Ptr Parser::assignment() {
	Expression::Ptr expr = equality();

	if(matchAndAdvance(TokenType::EQUAL)) {
		auto accessExpression = std::dynamic_pointer_cast<AccessExpr>(expr);
		auto variableExpression = std::dynamic_pointer_cast<VariableExpr>(expr);
		if(accessExpression) {	// FIXME: May god forgive me for using std::dynamic_pointer_cast
			Expression::Ptr newValue = assignment();
			Position position(expr->getPosition(), newValue->getPosition());
			expr = Expression::makePtr<AssignmentExpr>(position,
					accessExpression->getOwner(),
					accessExpression->getName(),
					newValue);
		} else if(variableExpression) {
			Expression::Ptr newValue = assignment();
			Position position(expr->getPosition(), newValue->getPosition());
			expr = Expression::makePtr<AssignmentExpr>(position, nullptr, variableExpression->getName(), newValue);
		}
	}

	return expr;
}

Expression::Ptr Parser::equality() {
	Expression::Ptr expr = comparison();

	while(true) {
		if(!matchAndAdvance({TokenType::BANG_EQUAL, TokenType::EQUAL_EQUAL}))
			break;
		Token oper = previous();
		Expression::Ptr rhs = comparison();
		expr = Expression::makePtr<BinaryExpr>(Position(expr->getPosition(), rhs->getPosition()), expr, oper, rhs);
	}

	return expr;
}

Expression::Ptr Parser::comparison() {
	Expression::Ptr expr = term();

	while(true) {
		if(!(matchAndAdvance({TokenType::GREATER, TokenType::GREATER_EQUAL, TokenType::LESS, TokenType::LESS_EQUAL})))
			break;
		Token oper = previous();
		Expression::Ptr rhs = term();
		expr = Expression::makePtr<BinaryExpr>(Position(expr->getPosition(), rhs->getPosition()), expr, oper, rhs);
	}

	return expr;
}

Expression::Ptr Parser::term() {
	Expression::Ptr expr = factor();

	while(true) {
		if(!(matchAndAdvance({TokenType::MINUS, TokenType::PLUS})))
			break;
		Token oper = previous();
		Expression::Ptr rhs = factor();
		expr = Expression::makePtr<BinaryExpr>(Position(expr->getPosition(), rhs->getPosition()), expr, oper, rhs);
	}

	return expr;
}

Expression::Ptr Parser::factor() {
	Expression::Ptr expr = unary();

	while(true) {
		if(!(matchAndAdvance({TokenType::SLASH, TokenType::STAR})))
			break;
		Token oper = previous();
		Expression::Ptr rhs = unary();
		expr = Expression::makePtr<BinaryExpr>(Position(expr->getPosition(), rhs->getPosition()), expr, oper, rhs);
	}

	return expr;
}

Expression::Ptr Parser::unary() {
	if(!(matchAndAdvance({TokenType::BANG, TokenType::MINUS})))
		return call();

	Token oper = previous();
	Expression::Ptr rhs = unary();
	return Expression::makePtr<UnaryExpr>(Position(oper.getPosition(), rhs->getPosition()), oper, rhs);
}

Expression::Ptr Parser::call() {
	Expression::Ptr expr = primary();

	while(true) {
		if(matchAndAdvance(TokenType::DOT)) {
			Token name = consume(TokenType::NAME);
			expr = Expression::makePtr<AccessExpr>(Position(expr->getPosition(), name.getPosition()), expr, name);
			continue;
		}
		if(matchAndAdvance(TokenType::LEFT_PAREN)) {
			std::vector<Expression::Ptr> arguments;
			if(!match(TokenType::RIGHT_PAREN)) {
				do {
					arguments.push_back(assignment());
				} while(matchAndAdvance(TokenType::COMMA));
			}
			const Token& rightParen = consume(TokenType::RIGHT_PAREN);
			expr = Expression::makePtr<CallExpr>(Position(expr->getPosition(), rightParen.getPosition()),
					expr,
					arguments);
			continue;
		}
		break;
	}

	return expr;
}

Expression::Ptr Parser::primary() {
	if(match(TokenType::LEFT_PAREN)) {
		const Token& leftParen = consume(TokenType::LEFT_PAREN);
		Expression::Ptr expr = assignment();
		const Token& rightParen = consume(TokenType::RIGHT_PAREN);
		return Expression::makePtr<GroupExpr>(Position(leftParen.getPosition(), rightParen.getPosition()), expr);
	}
	if(match(TokenType::NOTHING)) {
		Position position = consume(TokenType::NOTHING).getPosition();
		return Expression::makePtr<LiteralExpr>(position, Value::makePtr<Nothing>());
	}
	if(match(TokenType::NUMBER)) {
		Position position = consume(TokenType::NUMBER).getPosition();
		return Expression::makePtr<LiteralExpr>(position, Value::makePtr<Number>(previous().getValue().asInt()));
	}
	if(match(TokenType::STRING)) {
		Position position = consume(TokenType::STRING).getPosition();
		return Expression::makePtr<LiteralExpr>(position, Value::makePtr<String>(previous().getValue().asString()));
	}
	if(match(TokenType::TRUE)) {
		Position position = consume(TokenType::TRUE).getPosition();
		return Expression::makePtr<LiteralExpr>(position, Value::makePtr<Bool>(true));
	}
	if(match(TokenType::FALSE)) {
		Position position = consume(TokenType::FALSE).getPosition();
		return Expression::makePtr<LiteralExpr>(position, Value::makePtr<Bool>(false));
	}
	if(match(TokenType::NAME)) {
		Position position = consume(TokenType::NAME).getPosition();
		return Expression::makePtr<VariableExpr>(position, previous());
	}
	if(match(TokenType::FUN))
		return function();
	if(match(TokenType::INSTANTIATE))
		return instantiation();

	if(previous().getType() == TokenType::END_OF_FILE)
		throw ParserException(peek().getPosition(), "Expected expression before " + peek().getLexeme());
	else
		throw ParserException(previous().getPosition(), "Expected expression after " + previous().getLexeme(), true);
}

Expression::Ptr Parser::function() {
	const Token& funKW = consume(TokenType::FUN);

	std::vector<FunctionExpr::Parameter> parameters;
	consume(TokenType::LEFT_PAREN);
	if(!match(TokenType::RIGHT_PAREN)) {
		do {
			Token name = consume(TokenType::NAME);
			consume(TokenType::COLON);
			std::string type = consume(TokenType::NAME).getValue().asString();
			parameters.emplace_back(name, type);
		} while(matchAndAdvance(TokenType::COMMA));
	}
	consume(TokenType::RIGHT_PAREN);
	consume(TokenType::ARROW);

	Expression::Ptr returnType = typeIndicator();
	Statement::Ptr implementation = block();

	Position position(funKW.getPosition(), implementation->getPosition());
	return Expression::makePtr<FunctionExpr>(position, parameters, returnType, implementation);
}

Expression::Ptr Parser::instantiation() {
	const Token& instantiateKW = consume(TokenType::INSTANTIATE);
	std::string name = consume(TokenType::NAME).getValue().asString();

	std::vector<Expression::Ptr> parameters;
	consume(TokenType::LEFT_PAREN);
	if(!match(TokenType::RIGHT_PAREN)) {
		do {
			parameters.emplace_back(assignment());
		} while(matchAndAdvance(TokenType::COMMA));
	}
	const Token& rightParen = consume(TokenType::RIGHT_PAREN);

	Position position(instantiateKW.getPosition(), rightParen.getPosition());
	return Expression::makePtr<InstantiationExpr>(position, name, parameters);
}

Expression::Ptr Parser::typeIndicator() {
	Token typeName = consume(TokenType::NAME);
	if(typeName.getValue().asString() == Value::NativeTypes::Function) {
		consume(TokenType::LEFT_PAREN);
		std::vector<Expression::Ptr> paramTypes;
		if(!match(TokenType::RIGHT_PAREN)) {
			do {
				paramTypes.push_back(typeIndicator());
			} while(matchAndAdvance(TokenType::COMMA));
		}
		consume(TokenType::RIGHT_PAREN);
		consume(TokenType::ARROW);
		Expression::Ptr returnType = typeIndicator();

		Position position(typeName.getPosition(), returnType->getPosition());
		return Expression::makePtr<TypeExpr>(position, paramTypes, returnType);
	}
	return Expression::makePtr<TypeExpr>(typeName.getPosition(), typeName.getValue().asString());
}

Token Parser::advance() { return m_tokens[m_current++]; }

Token Parser::consume(TokenType expected) { return consume({expected}); }

Token Parser::consume(std::initializer_list<TokenType> expected) {
	std::string name;
	for(const auto& e : expected) {
		if(!isAtEnd() && peek().getType() == e)
			return advance();
		name += TokenTypeName::getFor(e) + " ";
	}

	bool unfinished = peek().getType() == TokenType::END_OF_FILE;
	throw ParserException(peek().getPosition(),
			"Expected " + name + "but got " + TokenTypeName::getFor(peek().getType()),
			unfinished);
}

Token Parser::peek() const { return m_tokens[m_current]; }

bool Parser::match(TokenType expected) const { return match({expected}); }
bool Parser::matchAndAdvance(TokenType expected) { return matchAndAdvance({expected}); }

bool Parser::match(std::initializer_list<TokenType> expected) const {
	if(isAtEnd())
		return false;
	return std::any_of(expected.begin(), expected.end(), [this](TokenType type) { return type == peek().getType(); });
}

bool Parser::matchAndAdvance(std::initializer_list<TokenType> expected) {
	bool res = match(expected);
	if(res)
		advance();
	return res;
}

Token Parser::previous() const { return m_tokens[m_current - 1]; }

bool Parser::isAtEnd() const { return peek().getType() == TokenType::END_OF_FILE; }
}