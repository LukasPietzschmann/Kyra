#include <gtest/gtest.h>

#include "../src/LibKyra/Lexer.hpp"
#include "../src/LibKyra/Position.hpp"

TEST(lexer, AllTokens) {
	Kyra::Lexer lexer("(){},;.-+/*= ==! != < <= > >= -> : name\"string\"123var fun print nothing true false return");
	std::vector<Kyra::TokenType> expected{Kyra::TokenType::LEFT_PAREN,
			Kyra::TokenType::RIGHT_PAREN,
			Kyra::TokenType::LEFT_CURLY,
			Kyra::TokenType::RIGHT_CURLY,
			Kyra::TokenType::COMMA,
			Kyra::TokenType::SEMICOLON,
			Kyra::TokenType::DOT,
			Kyra::TokenType::MINUS,
			Kyra::TokenType::PLUS,
			Kyra::TokenType::SLASH,
			Kyra::TokenType::STAR,
			Kyra::TokenType::EQUAL,
			Kyra::TokenType::EQUAL_EQUAL,
			Kyra::TokenType::BANG,
			Kyra::TokenType::BANG_EQUAL,
			Kyra::TokenType::LESS,
			Kyra::TokenType::LESS_EQUAL,
			Kyra::TokenType::GREATER,
			Kyra::TokenType::GREATER_EQUAL,
			Kyra::TokenType::ARROW,
			Kyra::TokenType::COLON,
			Kyra::TokenType::NAME,
			Kyra::TokenType::STRING,
			Kyra::TokenType::NUMBER,
			Kyra::TokenType::VAR,
			Kyra::TokenType::FUN,
			Kyra::TokenType::PRINT,
			Kyra::TokenType::NOTHING,
			Kyra::TokenType::TRUE,
			Kyra::TokenType::FALSE,
			Kyra::TokenType::RETURN,
			Kyra::TokenType::END_OF_FILE};
	const auto& actual = lexer.scanTokens();
	ASSERT_EQ(expected.size(), actual.size())
			<< "The number of parsed Tokens does not match the number of expected Tokens";
	for(unsigned long i = 0; i < actual.size(); ++i)
		EXPECT_EQ(actual[i].getType(), expected[i]);
}

TEST(lexer, EmptyInput) {
	Kyra::Lexer lexer("");
	const auto& tokens = lexer.scanTokens();
	ASSERT_EQ(tokens.size(), 1) << "Only one Token should be generated";
	EXPECT_EQ(tokens[0].getType(), Kyra::TokenType::END_OF_FILE);
}

TEST(lexer, MultiLineString) {
	Kyra::Lexer lexer(R"("Multiline
String")");
	auto tokens = lexer.scanTokens();
	ASSERT_EQ(tokens.size(), 2);
	EXPECT_EQ(tokens[0],
			Kyra::Token(Kyra::TokenType::STRING,
					Kyra::Position(1, 1, 2, 19),
					"\"Multiline\nString\"",
					"Multiline\nString"));
}

TEST(lexer, DifferKeywordFromIdentifier) {
	Kyra::Lexer lexer("return keyword");
	auto tokens = lexer.scanTokens();
	ASSERT_EQ(tokens.size(), 3);
	EXPECT_EQ(tokens[0].getType(), Kyra::TokenType::RETURN);
	EXPECT_EQ(tokens[1].getType(), Kyra::TokenType::NAME);
}