#include <iostream>
#include <fstream>
#include "LibSlg/Lexer.hpp"
#include "LibSlg/Parser.hpp"
#include "LibSlg/AstLogger.hpp"

int main(int argc, char** argv) {
	if(argc < 2) {
		const std::string prompt = "SLG > ";
		// REPL
		std::cout << prompt;
		for(std::string line; std::getline(std::cin, line);) {
			LibSlg::Lexer lexer(line);

			auto tokens = lexer.scanTokens();
			for(const auto& token : tokens)
				std::cout << token << std::endl;

			LibSlg::Parser parser(tokens);
			for(const auto& statement : parser.parse()) {
				LibSlg::AstLogger logger;
				logger.logStatement(statement);
			}

			std::cout << prompt;
		}
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