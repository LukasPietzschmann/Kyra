#include <gtest/gtest.h>

#include "../src/LibSlg/Lexer.hpp"
#include "../src/LibSlg/Position.hpp"

TEST(lexer, AllTokens) {
	Slanguage::Lexer lexer(
			"(){},;.-+/*= ==! != < <= > >= -> : name\"string\"123var fun print nothing true false return");
	std::vector<Slanguage::TokenType> expected{Slanguage::TokenType::LEFT_PAREN,
			Slanguage::TokenType::RIGHT_PAREN,
			Slanguage::TokenType::LEFT_CURLY,
			Slanguage::TokenType::RIGHT_CURLY,
			Slanguage::TokenType::COMMA,
			Slanguage::TokenType::SEMICOLON,
			Slanguage::TokenType::DOT,
			Slanguage::TokenType::MINUS,
			Slanguage::TokenType::PLUS,
			Slanguage::TokenType::SLASH,
			Slanguage::TokenType::STAR,
			Slanguage::TokenType::EQUAL,
			Slanguage::TokenType::EQUAL_EQUAL,
			Slanguage::TokenType::BANG,
			Slanguage::TokenType::BANG_EQUAL,
			Slanguage::TokenType::LESS,
			Slanguage::TokenType::LESS_EQUAL,
			Slanguage::TokenType::GREATER,
			Slanguage::TokenType::GREATER_EQUAL,
			Slanguage::TokenType::ARROW,
			Slanguage::TokenType::COLON,
			Slanguage::TokenType::NAME,
			Slanguage::TokenType::STRING,
			Slanguage::TokenType::NUMBER,
			Slanguage::TokenType::VAR,
			Slanguage::TokenType::FUN,
			Slanguage::TokenType::PRINT,
			Slanguage::TokenType::NOTHING,
			Slanguage::TokenType::TRUE,
			Slanguage::TokenType::FALSE,
			Slanguage::TokenType::RETURN,
			Slanguage::TokenType::END_OF_FILE};
	const auto& actual = lexer.scanTokens();
	ASSERT_EQ(expected.size(), actual.size())
			<< "The number of parsed Tokens does not match the number of expected Tokens";
	for(unsigned long i = 0; i < actual.size(); ++i)
		EXPECT_EQ(actual[i].getType(), expected[i]);
}

TEST(lexer, EmptyInput) {
	Slanguage::Lexer lexer("");
	const auto& tokens = lexer.scanTokens();
	ASSERT_EQ(tokens.size(), 1) << "Only one Token should be generated";
	EXPECT_EQ(tokens[0].getType(), Slanguage::TokenType::END_OF_FILE);
}

TEST(lexer, MultiLineString) {
	Slanguage::Lexer lexer(R"("Multiline
String")");
	auto tokens = lexer.scanTokens();
	ASSERT_EQ(tokens.size(), 2);
	EXPECT_EQ(tokens[0],
			Slanguage::Token(Slanguage::TokenType::STRING,
					Slanguage::Position(1, 1, 2, 19),
					"\"Multiline\nString\"",
					"Multiline\nString"));
}

TEST(lexer, DifferKeywordFromIdentifier) {
	Slanguage::Lexer lexer("return keyword");
	auto tokens = lexer.scanTokens();
	ASSERT_EQ(tokens.size(), 3);
	EXPECT_EQ(tokens[0].getType(), Slanguage::TokenType::RETURN);
	EXPECT_EQ(tokens[1].getType(), Slanguage::TokenType::NAME);
}