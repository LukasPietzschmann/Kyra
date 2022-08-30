#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

#include "AST.hpp"
#include "ASTPrinter.hpp"
#include "Aliases.hpp"
#include "Lexer.hpp"
#include "Parser.hpp"
#include "SourceRange.hpp"
#include "Token.hpp"

int main(int argc, char** argv) {
	if(argc != 2)
		return 1;

	std::filesystem::path source_file_path(argv[1]);
	if(!std::filesystem::exists(source_file_path))
		return 2;

	std::ifstream input_file_stream(source_file_path);
	std::string source_code((std::istreambuf_iterator<char>(input_file_stream)), (std::istreambuf_iterator<char>()));

	const std::vector<Token>& tokens = Lexer::the().scan_input(source_code);
	const std::vector<RefPtr<Statement>> statements = Parser::the().parse_tokens(tokens);
	ASTPrinter printer;
	for(const RefPtr<Statement>& statement : statements)
		printer.print(*statement);

	return 0;
}
