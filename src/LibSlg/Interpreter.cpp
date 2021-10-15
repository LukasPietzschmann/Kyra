#include "Interpreter.hpp"

namespace LibSlg {
void Interpreter::execute(const std::string& code, bool verboseLogging, bool passThroughExceptions) {
	try {
		auto tokens = Lexer(code).scanTokens();
		auto statements = Parser(tokens).parse();
		if(verboseLogging) {
			AstLogger logger;
			for(const auto& statement : statements)
				logger.logStatement(statement);
		}

		TypeChecker::Result result = TypeChecker::getInstance().check(statements);
		for(const auto& error : result.getErrors()) {
			if(passThroughExceptions)
				throw TypingException(error);
			std::cout << "[Typing Error] " << error << "\n";
		}
		std::cout << std::flush;
		if(result.hasErrors())
			return;

		for(const auto& statement : statements)
			Runtime::getInstance().executeStatement(statement);
	} catch(const ParserException& exception) {
		if(passThroughExceptions)
			throw ParserException(exception);
		std::cout << "[Parser Error] " << exception.what() << std::endl;
	} catch(const LexerException& exception) {
		if(passThroughExceptions)
			throw LexerException(exception);
		std::cout << "[Lexer Error] " << exception.what() << std::endl;
	}
}

bool Interpreter::isIncompleteStatement(const std::string& code) {
	try {
		Parser parser(Lexer(code).scanTokens());
		parser.parse();
	} catch(const ParserException& exception) { return exception.isUnfinished(); } catch(...) {
		return false;
	}
	return false;
}
}