#pragma once

#include <string>

#include "AstLogger.hpp"
#include "Lexer.hpp"
#include "Parser.hpp"
#include "Runtime/Runtime.hpp"
#include "Typing/TypeChecker.hpp"

namespace LibSlg {
class Interpreter {
public:
	static void execute(const std::string& code, bool verboseLogging = false, bool passThroughExceptions = false);
	static bool isIncompleteStatement(const std::string& code);
};
}