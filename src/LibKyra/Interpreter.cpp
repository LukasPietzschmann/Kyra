#include "include/Interpreter.hpp"

#include <iostream>
#include <memory>

#include "AstLogger.hpp"
#include "Exceptions.hpp"
#include "Lexer.hpp"
#include "Parser.hpp"
#include "Runtime/Runtime.hpp"
#include "Typing/TypeChecker.hpp"

namespace Kyra {
void Interpreter::execute(const std::string& code, bool verbose_logging, bool pass_through_exceptions) {
	try {
		const std::vector<Token>& tokens = Lexer(code).scan_tokens();
		const std::vector<Statement::Ptr>& statements = Parser(tokens).parse();
		if(verbose_logging) {
			AstLogger logger;
			for(const Statement::Ptr& statement : statements)
				logger.log_statement(statement);
		}

		const TypeChecker::Result& result = TypeChecker::the().check(statements);
		for(const std::string& error : result.get_errors()) {
			if(pass_through_exceptions)
				throw TypingException(error);
			std::cout << "[Typing Error] " << error << "\n";
		}
		std::cout << std::flush;
		if(result.has_errors())
			return;

		for(const Statement::Ptr& statement : statements)
			Runtime::the().execute_statement(statement);
	} catch(const ParserException& exception) {
		if(pass_through_exceptions)
			throw ParserException(exception);
		std::cout << "[Parser Error] " << exception.what() << std::endl;
	} catch(const LexerException& exception) {
		if(pass_through_exceptions)
			throw LexerException(exception);
		std::cout << "[Lexer Error] " << exception.what() << std::endl;
	}
}

bool Interpreter::is_incomplete_statement(const std::string& code) {
	try {
		Parser parser(Lexer(code).scan_tokens());
		parser.parse();
	} catch(const ParserException& exception) { return exception.is_unfinished(); } catch(const MessageException&) {
		return false;
	}
	return false;
}
}