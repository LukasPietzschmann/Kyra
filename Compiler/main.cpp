#include <filesystem>
#include <fstream>
#include <iostream>
#include <optional>
#include <string>

#include "AST.hpp"
#include "ASTPrinter.hpp"
#include "Aliases.hpp"
#include "Lexer.hpp"
#include "Parser.hpp"
#include "SourceRange.hpp"
#include "Token.hpp"
#include "TypeChecker.hpp"

int main(int argc, char** argv) {
	if(argc != 2)
		return 1;

	std::filesystem::path source_file_path(argv[1]);
	if(!std::filesystem::exists(source_file_path))
		return 2;

	std::ifstream input_file_stream(source_file_path);
	std::string source_code((std::istreambuf_iterator<char>(input_file_stream)), (std::istreambuf_iterator<char>()));
	input_file_stream.close();

	const ErrorOr<std::vector<Token>>& error_or_tokens = Lexer::the().scan_input(source_code, source_file_path);
	if(error_or_tokens.is_error()) {
		error_or_tokens.get_exception().print(std::cout);
		return 1;
	}

	const ErrorOr<std::vector<RefPtr<Statement>>>& error_or_statements =
		Parser::the().parse_tokens(error_or_tokens.get_result());
	if(error_or_statements.is_error()) {
		error_or_statements.get_exception().print(std::cout);
		return 1;
	}
	for(const RefPtr<Statement>& statement : error_or_statements.get_result()) {
		const ErrorOr<void>& maybe_error = TypeChecker::the().check_statement(*statement);
		if(maybe_error.is_error())
			maybe_error.get_exception().print(std::cout);
	}

	return 0;
}
