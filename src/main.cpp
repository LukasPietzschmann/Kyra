#include <config.h>

#include <cxxopts.hpp>
#include <fstream>
#include <iostream>

#include "LibSlg/Interpreter.hpp"

#ifdef HAS_READLINE
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
	options.add_options()("f,file", "The file to execute", cxxopts::value<std::string>())("verbose",
			"Prints as much information as available",
			cxxopts::value<bool>(verbose))("v,version", "Prints the version number")("h,help", "Prints this message");
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
			std::cout << "Slanguage " << PROJECT_VERSION << " by Lukas Pietzschmann"
					  << "\n";
			std::cout << R"(Type "exit" or press "CTRL-C" to exit the REPL)" << std::endl;
#ifdef HAS_READLINE
			niceRepl();
#else
			simpleRepl();
#endif
		} else {
			// File
			std::stringstream fileContent;
			std::string line;
			std::ifstream fileStream(result["file"].as<std::string>());
			while(std::getline(fileStream, line))
				fileContent << line << "\n";
			fileStream.close();

			LibSlg::Interpreter::getInstance().execute(fileContent.str(), verbose);
		}
	} catch(cxxopts::OptionException& exception) {
		std::cout << exception.what() << "\n" << options.help() << std::endl;
		return 1;
	}

	return 0;
}

#ifndef HAS_READLINE
void simpleRepl() {
	std::cout << "Btw: To get the full REPL experience install the GNU readline library!" << std::endl;
	std::cout << PROMPT;
	for(std::string line; std::getline(std::cin, line);) {
		if(line == "exit")
			break;

		LibSlg::Interpreter::getInstance().execute(line, verbose);

		std::cout << PROMPT;
	}
}
#endif

#ifdef HAS_READLINE
void niceRepl() {
	static bool unfinished = false;
	static std::string completeCode;
	static std::string prompt = PROMPT;

	rl_bind_key('\t', rl_insert);

	while(true) {
		char* inputLine = readline(prompt.c_str());
		if(!inputLine || std::strcmp(inputLine, "exit") == 0) {
			free(inputLine);
			break;
		}

		completeCode += inputLine;
		unfinished = LibSlg::Interpreter::getInstance().isIncompleteStatement(completeCode);

		if(unfinished) {
			prompt = ".... ";
			completeCode += "\n";
		} else {
			add_history(completeCode.c_str());
			LibSlg::Interpreter::getInstance().execute(completeCode, verbose);
			completeCode.clear();
			prompt = PROMPT;
		}

		free(inputLine);
	}
}
#endif