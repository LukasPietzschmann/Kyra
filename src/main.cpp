#include <iostream>
#include <fstream>
#include <cxxopts.hpp>
#include <config.h>
#include "LibSlg/Interpreter.hpp"

#ifdef HAS_READLINE
#include <readline/history.h>
#include <readline/readline.h>
#endif

#define PROMPT "SLG > "

void simpleRepl();
void niceRepl();

bool verbose = false;

int main(int argc, char** argv) {
	std::ostream::sync_with_stdio(false);
	std::string desc("The Slanguage programming language");
	cxxopts::Options options(argv[0], desc + " - Version " + PROJECT_VERSION);
	options.add_options()
			("f,file", "The file to execute", cxxopts::value<std::string>())
			("verbose", "Prints as much information as available", cxxopts::value<bool>(verbose))
			("v,version", "Prints the version number")
			("h,help", "Prints this message");
	options.parse_positional({"file"});
	options.positional_help("[FILE]").show_positional_help();

	try {
		cxxopts::ParseResult result = options.parse(argc, argv);

		if(result.count("help")) {
			std::cout << options.help() << std::endl;
			return 0;
		}

		if(result.count("version")) {
			std::cout << "Slanguage " << PROJECT_VERSION << std::endl;
			return 0;
		}

		if(!result.count("file")) {
			std::cout << "Slanguage " << PROJECT_VERSION << " by Lukas Pietzschmann" << "\n";
			std::cout << R"(Type "exit" or press "CTRL-C" to exit the REPL)" << std::endl;
#ifdef HAS_READLINE
			niceRepl();
#else
			simpleRepl();
#endif
		}else {
			// File
			std::stringstream fileContent;
			std::string line;
			std::ifstream fileStream(result["file"].as<std::string>());
			while(std::getline(fileStream, line))
				fileContent << line << "\n";
			fileStream.close();

			LibSlg::Interpreter::getInstance().execute(fileContent.str(), verbose);
		}
	}catch(cxxopts::OptionException& exception) {
		std::cout << exception.what() << "\n" << options.help() << std::endl;
		return 1;
	}

	return 0;
}

#ifndef HAS_READLINE
void simpleRepl() {
	std::cout << PROMPT;
	for(std::string line; std::getline(std::cin, line);) {
		if(line == "exit")
			break;

		LibSlg::Interpreter::getInstance().execute(inputLine, verbose);

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

		LibSlg::Interpreter::getInstance().execute(inputLine, verbose);

		free(inputLine);
	}
}
#endif