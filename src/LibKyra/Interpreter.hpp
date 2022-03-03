#pragma once

#include <string>

namespace Kyra {
class Interpreter {
public:
	static void execute(const std::string& code, bool verbose_logging = false, bool pass_through_exceptions = false);
	static bool is_incomplete_statement(const std::string& code);
};
}