#include <Interpreter.hpp>
#include <cstring>
#include <cxxopts.hpp>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>

#include "version.h"

#ifdef HAS_READLINE
#include <readline/history.h>
#include <readline/readline.h>
#endif

void simple_repl();
void nice_repl();

bool verbose = false;

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
			nice_repl();
#else
			simple_repl();
#endif
		} else {
			// File
			std::stringstream file_content;
			std::string line;
			const auto& file_name = result["file"].as<std::string>();
			std::basic_fstream<char> file_stream(file_name);
			if(!file_stream.good()) {
				std::cout << "File " << file_name << " not found" << std::endl;
				return 1;
			}
			while(std::getline(file_stream, line))
				file_content << line << "\n";
			file_stream.close();

			Kyra::Interpreter::execute(file_content.str(), verbose);
		}
	} catch(const cxxopts::OptionException& exception) {
		std::cout << exception.what() << "\n" << options.help() << std::endl;
		return 1;
	}

	return 0;
}

#ifndef HAS_READLINE
void simple_repl() {
	std::cout << "Btw: To get the full REPL experience install the GNU readline library and recompile Kyra!"
			  << std::endl;
	std::cout << "KYRA > ";
	for(std::string line; std::getline(std::cin, line);) {
		if(line == "exit")
			break;
		Kyra::Interpreter::execute(line, verbose);
		std::cout << "KYRA > ";
	}
}
#endif

#ifdef HAS_READLINE
void nice_repl() {
	static bool unfinished = false;
	static std::string complete_code;
	static std::string prompt = "KYRA > ";

	rl_bind_key('\t', rl_insert);

	while(true) {
		char* input_line = readline(prompt.c_str());
		if(!input_line || std::strcmp(input_line, "exit") == 0) {
			delete[] input_line;
			break;
		}

		complete_code += input_line;
		unfinished = Kyra::Interpreter::is_incomplete_statement(complete_code);

		if(unfinished) {
			prompt = ".... ";
			complete_code += "\n";
		} else {
			add_history(complete_code.c_str());
			Kyra::Interpreter::execute(complete_code, verbose);
			complete_code.clear();
			prompt = "KYRA > ";
		}

		delete[] input_line;
	}
}
#endif