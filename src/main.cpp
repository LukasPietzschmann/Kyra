#include <iostream>
#include <fstream>
#include "LibSlg/Lexer.hpp"
#include "LibSlg/Parser.hpp"
#include "LibSlg/AstLogger.hpp"

#ifdef HAS_READLINE
#include <readline/history.h>
#include <readline/readline.h>
#endif

#define PROMPT "SLG > "

void simpleRepl();
void niceRepl();

int main(int argc, char** argv) {
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

		LibSlg::Lexer lexer(fileContent.str());
		auto tokens = lexer.scanTokens();
		for(const auto& token : tokens)
			std::cout << token << std::endl;

		LibSlg::Parser parser(tokens);
		for(const auto& statement : parser.parse()) {
			LibSlg::AstLogger logger;
			logger.logStatement(statement);
		}
	}

	return 0;
}

void simpleRepl() {
	std::cout << PROMPT;
	for(std::string line; std::getline(std::cin, line);) {
		if(line == "exit")
			break;

		LibSlg::Lexer lexer(line);

		auto tokens = lexer.scanTokens();
		for(const auto& token : tokens)
			std::cout << token << std::endl;

		LibSlg::Parser parser(tokens);
		for(const auto& statement : parser.parse()) {
			LibSlg::AstLogger logger;
			logger.logStatement(statement);
		}

		std::cout << PROMPT;
	}
}

void niceRepl() {
	while(true) {
		char* inputLine = readline(PROMPT);
		if(!inputLine || std::strcmp(inputLine, "exit") == 0) {
			free(inputLine);
			break;
		}
		if(*inputLine)
			add_history(inputLine);

		LibSlg::Lexer lexer(inputLine);
		auto tokens = lexer.scanTokens();
		for(const auto& token : tokens)
			std::cout << token << std::endl;

		LibSlg::Parser parser(tokens);
		for(const auto& statement : parser.parse()) {
			LibSlg::AstLogger logger;
			logger.logStatement(statement);
		}

		free(inputLine);
	}
}