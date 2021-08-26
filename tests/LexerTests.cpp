#include <gtest/gtest.h>
#include "../src/LibSlg/Lexer.hpp"

TEST(lexer, AllTokens) {
	LibSlg::Lexer lexer("(){},;.-+/*= ==! != < <= > >=name\"string\"123var fun print nothing true false return");
	std::vector<LibSlg::Token> expected {
			LibSlg::Token(LibSlg::TokenType::LEFT_PAREN, 0, "("),
			LibSlg::Token(LibSlg::TokenType::RIGHT_PAREN, 0, ")"),
			LibSlg::Token(LibSlg::TokenType::LEFT_CURLY, 0, "{"),
			LibSlg::Token(LibSlg::TokenType::RIGHT_CURLY, 0, "}"),
			LibSlg::Token(LibSlg::TokenType::COMMA, 0, ","),
			LibSlg::Token(LibSlg::TokenType::SEMICOLON, 0, ";"),
			LibSlg::Token(LibSlg::TokenType::DOT, 0, "."),
			LibSlg::Token(LibSlg::TokenType::MINUS, 0, "-"),
			LibSlg::Token(LibSlg::TokenType::PLUS, 0, "+"),
			LibSlg::Token(LibSlg::TokenType::SLASH, 0, "/"),
			LibSlg::Token(LibSlg::TokenType::STAR, 0, "*"),
			LibSlg::Token(LibSlg::TokenType::EQUAL, 0, "="),
			LibSlg::Token(LibSlg::TokenType::EQUAL_EQUAL, 0, "=="),
			LibSlg::Token(LibSlg::TokenType::BANG, 0, "!"),
			LibSlg::Token(LibSlg::TokenType::BANG_EQUAL, 0, "!="),
			LibSlg::Token(LibSlg::TokenType::LESS, 0, "<"),
			LibSlg::Token(LibSlg::TokenType::LESS_EQUAL, 0, "<="),
			LibSlg::Token(LibSlg::TokenType::GREATER, 0, ">"),
			LibSlg::Token(LibSlg::TokenType::GREATER_EQUAL, 0, ">="),
			LibSlg::Token(LibSlg::TokenType::NAME, 0, "name", "name"),
			LibSlg::Token(LibSlg::TokenType::STRING, 0, "\"string\"", "string"),
			LibSlg::Token(LibSlg::TokenType::NUMBER, 0, "123", "123"),
			LibSlg::Token(LibSlg::TokenType::VAR, 0, "var"),
			LibSlg::Token(LibSlg::TokenType::FUN, 0, "fun"),
			LibSlg::Token(LibSlg::TokenType::PRINT, 0, "print"),
			LibSlg::Token(LibSlg::TokenType::NOTHING, 0, "nothing"),
			LibSlg::Token(LibSlg::TokenType::TRUE, 0, "true"),
			LibSlg::Token(LibSlg::TokenType::FALSE, 0, "false"),
			LibSlg::Token(LibSlg::TokenType::RETURN, 0, "return"),
			LibSlg::Token(LibSlg::TokenType::END_OF_FILE, 0, "")
	};
	auto actual = lexer.scanTokens();
	ASSERT_EQ(expected.size(), actual.size())
								<< "The number of parsed Tokens does not match the number of expected Tokens";
	for(int i = 0; i < actual.size(); ++i)
		EXPECT_EQ(actual[i], expected[i]);
}

TEST(lexer, EmptyInput) {
	LibSlg::Lexer lexer("");
	auto tokens = lexer.scanTokens();
	ASSERT_EQ(tokens.size(), 1) << "Only one Token should be generated";
	EXPECT_EQ(tokens[0], LibSlg::Token(LibSlg::TokenType::END_OF_FILE, 0, ""));
}

TEST(lexer, MultiLineString) {
	LibSlg::Lexer lexer(R"("Multiline
String")");
	auto tokens = lexer.scanTokens();
	ASSERT_EQ(tokens.size(), 2);
	EXPECT_EQ(tokens[0], LibSlg::Token(LibSlg::TokenType::STRING, 1, "\"Multiline\nString\"", "Multiline\nString"));
}

TEST(lexer, DifferKeywordFromIdentifier) {
	LibSlg::Lexer lexer("return keyword");
	auto tokens = lexer.scanTokens();
	ASSERT_EQ(tokens.size(), 3);
	EXPECT_EQ(tokens[0].getType(), LibSlg::TokenType::RETURN);
	EXPECT_EQ(tokens[1].getType(), LibSlg::TokenType::NAME);
}