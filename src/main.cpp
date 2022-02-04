#include <config.h>

#include <cstring>
#include <cxxopts.hpp>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>

#include "LibKyra/Interpreter.hpp"

#ifdef HAS_READLINE
#include <readline/history.h>
#include <readline/readline.h>
#endif

void simpleRepl();
void niceRepl();

static bool verbose = false;
static std::string defaultPrompt = "KYRA > ";

int main(int argc, char** argv) {
	std::ostream::sync_with_stdio(false);
	std::string desc("The Kyra programming language");
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
			std::cout << "Kyra " << PROJECT_VERSION << std::endl;
			return 0;
		}

		if(!result.count("file")) {
			std::cout << "Kyra " << PROJECT_VERSION << " by Lukas Pietzschmann"
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
			const auto& fileName = result["file"].as<std::string>();
			std::basic_fstream<char> fileStream(fileName);
			if(!fileStream.good()) {
				std::cout << "File " << fileName << " not found" << std::endl;
				return 1;
			}
			while(std::getline(fileStream, line))
				fileContent << line << "\n";
			fileStream.close();

			Kyra::Interpreter::execute(fileContent.str(), verbose);
		}
	} catch(const cxxopts::OptionException& exception) {
		std::cout << exception.what() << "\n" << options.help() << std::endl;
		return 1;
	}

	return 0;
}

#ifndef HAS_READLINE
void simpleRepl() {
	std::cout << "Btw: To get the full REPL experience install the GNU readline library!" << std::endl;
	std::cout << defaultPrompt;
	for(std::string line; std::getline(std::cin, line);) {
		if(line == "exit")
			break;

		Kyra::Interpreter::getInstance().execute(line, verbose);

		std::cout << defaultPrompt;
	}
}
#endif

#ifdef HAS_READLINE
void niceRepl() {
	static bool unfinished = false;
	static std::string completeCode;
	static std::string prompt = defaultPrompt;

	rl_bind_key('\t', rl_insert);

	while(true) {
		char* inputLine = readline(prompt.c_str());
		if(!inputLine || std::strcmp(inputLine, "exit") == 0) {
			delete[] inputLine;
			break;
		}

		completeCode += inputLine;
		unfinished = Kyra::Interpreter::isIncompleteStatement(completeCode);

		if(unfinished) {
			prompt = ".... ";
			completeCode += "\n";
		} else {
			add_history(completeCode.c_str());
			Kyra::Interpreter::execute(completeCode, verbose);
			completeCode.clear();
			prompt = defaultPrompt;
		}

		delete[] inputLine;
	}
}
#endif