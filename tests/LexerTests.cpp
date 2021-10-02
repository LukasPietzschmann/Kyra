#include <gtest/gtest.h>

#include "../src/LibSlg/Lexer.hpp"
#include "../src/LibSlg/Position.hpp"

TEST(lexer, AllTokens) {
	LibSlg::Lexer lexer("(){},;.-+/*= ==! != < <= > >= -> : name\"string\"123var fun print nothing true false return");
	std::vector<LibSlg::TokenType> expected{LibSlg::TokenType::LEFT_PAREN,
			LibSlg::TokenType::RIGHT_PAREN,
			LibSlg::TokenType::LEFT_CURLY,
			LibSlg::TokenType::RIGHT_CURLY,
			LibSlg::TokenType::COMMA,
			LibSlg::TokenType::SEMICOLON,
			LibSlg::TokenType::DOT,
			LibSlg::TokenType::MINUS,
			LibSlg::TokenType::PLUS,
			LibSlg::TokenType::SLASH,
			LibSlg::TokenType::STAR,
			LibSlg::TokenType::EQUAL,
			LibSlg::TokenType::EQUAL_EQUAL,
			LibSlg::TokenType::BANG,
			LibSlg::TokenType::BANG_EQUAL,
			LibSlg::TokenType::LESS,
			LibSlg::TokenType::LESS_EQUAL,
			LibSlg::TokenType::GREATER,
			LibSlg::TokenType::GREATER_EQUAL,
			LibSlg::TokenType::ARROW,
			LibSlg::TokenType::COLON,
			LibSlg::TokenType::NAME,
			LibSlg::TokenType::STRING,
			LibSlg::TokenType::NUMBER,
			LibSlg::TokenType::VAR,
			LibSlg::TokenType::FUN,
			LibSlg::TokenType::PRINT,
			LibSlg::TokenType::NOTHING,
			LibSlg::TokenType::TRUE,
			LibSlg::TokenType::FALSE,
			LibSlg::TokenType::RETURN,
			LibSlg::TokenType::END_OF_FILE};
	const auto& actual = lexer.scanTokens();
	ASSERT_EQ(expected.size(), actual.size())
			<< "The number of parsed Tokens does not match the number of expected Tokens";
	for(unsigned long i = 0; i < actual.size(); ++i)
		EXPECT_EQ(actual[i].getType(), expected[i]);
}

TEST(lexer, EmptyInput) {
	LibSlg::Lexer lexer("");
	const auto& tokens = lexer.scanTokens();
	ASSERT_EQ(tokens.size(), 1) << "Only one Token should be generated";
	EXPECT_EQ(tokens[0].getType(), LibSlg::TokenType::END_OF_FILE);
}

TEST(lexer, MultiLineString) {
	LibSlg::Lexer lexer(R"("Multiline
String")");
	auto tokens = lexer.scanTokens();
	ASSERT_EQ(tokens.size(), 2);
	EXPECT_EQ(tokens[0],
			LibSlg::Token(LibSlg::TokenType::STRING,
					LibSlg::Position(1, 1, 2, 19),
					"\"Multiline\nString\"",
					"Multiline\nString"));
}

TEST(lexer, DifferKeywordFromIdentifier) {
	LibSlg::Lexer lexer("return keyword");
	auto tokens = lexer.scanTokens();
	ASSERT_EQ(tokens.size(), 3);
	EXPECT_EQ(tokens[0].getType(), LibSlg::TokenType::RETURN);
	EXPECT_EQ(tokens[1].getType(), LibSlg::TokenType::NAME);
}