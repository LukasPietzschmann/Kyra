#include <iostream>
#include <fstream>
#include "LibSlg/AstLogger.hpp"
#include "LibSlg/Interpreter.hpp"

#ifdef HAS_READLINE
#include <readline/history.h>
#include <readline/readline.h>
#endif

#define PROMPT "SLG > "

void simpleRepl();
void niceRepl();

int main(int argc, char** argv) {
	std::ostream::sync_with_stdio(false);

	if(argc < 2) {
		std::cout << "Slanguage 0.9 by Lukas Pietzschmann" << "\n";
		std::cout << R"(Type "exit" or "CTRL-C" to exit the REPL)" << std::endl;
#ifdef HAS_READLINE
		if(argc > 1 && std::string(argv[1]) == "-d")
			rl_bind_key('\t', rl_insert);
		niceRepl();
#else
		simpleRepl();
#endif
	}else {
		// File
		std::stringstream fileContent;
		std::string line;
		std::ifstream fileStream(*(argv + 1));
		while(std::getline(fileStream, line))
			fileContent << line << "\n";
		fileStream.close();

		LibSlg::Interpreter::getInstance().execute(fileContent.str());
	}

	return 0;
}

#ifndef HAS_READLINE
void simpleRepl() {
	std::cout << PROMPT;
	for(std::string line; std::getline(std::cin, line);) {
		if(line == "exit")
			break;

		LibSlg::Interpreter::getInstance().execute(inputLine);

		std::cout << PROMPT;
	}
}
#endif

#ifdef HAS_READLINE
void niceRepl() {
	while(true) {
		char* inputLine = readline(PROMPT);
		if(!inputLine || std::strcmp(inputLine, "exit") == 0) {
			free(inputLine);
			break;
		}
		if(*inputLine)
			add_history(inputLine);

		LibSlg::Interpreter::getInstance().execute(inputLine);

		free(inputLine);
	}
}
#endif