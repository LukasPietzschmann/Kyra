#include <iostream>
#include "LibSlg/Lexer.hpp"

int main() {
	LibSlg::Lexer lexer("(){},;.-+/*	= == != !> >= < <=name  \"string\" 420 var fun object print nothing true false return");
	for(const auto &token : lexer.scanTokens())
		std::cout << token << std::endl;

	return 0;
}