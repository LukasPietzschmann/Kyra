#pragma once

#include <sstream>

#include "AST.hpp"
#include "Aliases.hpp"
#include "Error.hpp"
#include "Token.hpp"

class Parser {
public:
	static Parser& the() {
		static Parser instance;
		return instance;
	}

	Parser() = default;
	Parser(const Parser&) = delete;
	Parser(Parser&&) noexcept = default;

	Parser& operator=(const Parser&) = delete;
	Parser& operator=(Parser&&) noexcept = default;

	ErrorOr<std::vector<RefPtr<Statement>>> parse_tokens(const std::vector<Token>& tokens);

private:
	std::vector<RefPtr<Statement>> m_statements;
	const std::vector<Token>* m_tokens;
	std::vector<Token>::const_iterator m_current_token;

	RefPtr<Statement> statement();
	RefPtr<Statement> expression_statement();
	RefPtr<Statement> return_statement();
	RefPtr<Statement> block();
	RefPtr<Statement> declaration();
	RefPtr<Statement> variable_declaration();
	RefPtr<Statement> function_declaration();

	RefPtr<Expression> expression();
	RefPtr<Expression> assignment();
	RefPtr<Expression> term();
	RefPtr<Expression> factor();
	RefPtr<Expression> call();
	RefPtr<Expression> primary();
	RefPtr<Expression> type();

	template <typename... Args, typename = All<TokenType, Args...>>
	bool match(Args... args) const {
		return ((m_current_token->get_type() == args) || ...);
	}

	template <typename... Args, typename = All<TokenType, Args...>>
	bool match_and_advance(Args... args) {
		if(!match(args...))
			return false;
		++m_current_token;
		return true;
	}

	template <typename... Args, typename = All<TokenType, Args...>>
	const Token& consume(Args... args) {
		const Token& current = *m_current_token;
		if(match_and_advance(args...)) {
			return current;
		}
		std::stringstream message;
		message << "Expexted ";
		(message << ... << (Token::get_name_for(args) + ", "))
			<< "but found " << Token::get_name_for(current.get_type());
		throw ErrorException(message.str(), m_current_token->get_source_range());
	}

	bool is_at_end() const;
};
