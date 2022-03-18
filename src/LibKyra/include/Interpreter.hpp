#pragma once

#include <string>

namespace Kyra {
class Interpreter {
public:
	static void execute(const std::string& code, bool verbose_logging = false);
};
}