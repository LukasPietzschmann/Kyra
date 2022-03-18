#include "include/Interpreter.hpp"

#include <iostream>
#include <memory>

#include "AstLogger.hpp"
#include "Exceptions.hpp"
#include "Lexer.hpp"
#include "Parser.hpp"
#include "Result.hpp"
#include "Runtime/Runtime.hpp"
#include "Typing/TypeChecker.hpp"

namespace Kyra {
void Interpreter::execute(const std::string& code, bool verbose_logging) {
	auto lexer_result = Lexer(code).scan_tokens();
	if(lexer_result.has_errors()) {
		for(const auto& error : lexer_result.get_errors())
			std::cout << "[Lexer Error] " << error << std::endl;
		return;
	}

	auto parser_result = Parser(lexer_result.assert_get_value()).parse();
	if(parser_result.has_errors()) {
		for(const auto& error : parser_result.get_errors())
			std::cout << "[Parser Error] " << error << std::endl;
		return;
	}

	const auto& type_checking_result = TypeChecker::the().check(parser_result.assert_get_value());
	if(type_checking_result.has_errors()) {
		for(const auto& error : type_checking_result.get_errors())
			std::cout << "[Typing Error]" << error << std::endl;
		return;
	}

	if(verbose_logging) {
		AstLogger logger;
		for(const Statement::Ptr& statement : parser_result.assert_get_value())
			logger.log_statement(statement);
	}

	for(const Statement::Ptr& statement : parser_result.assert_get_value())
		Runtime::the().execute_statement(statement);
}
}