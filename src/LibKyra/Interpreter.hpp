#pragma once

#include <string>

namespace Kyra {
class Interpreter {
public:
	static void execute(const std::string& code, bool verboseLogging = false, bool passThroughExceptions = false);
	static bool isIncompleteStatement(const std::string& code);
};
}