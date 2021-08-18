#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "../src/LibSlg/Parser.hpp"
#include "../src/LibSlg/Lexer.hpp"
#include "../src/LibSlg/AstJsonPrinter.hpp"

using ::testing::MatchesRegex;

class Parser : public testing::TestWithParam<std::string> {
protected:
	std::vector<LibSlg::Statement::Ptr> m_statements;
	LibSlg::AstJsonPrinter* m_jsonPrinter;

	void SetUp() override {
		auto tokens = LibSlg::Lexer(GetParam()).scanTokens();
		auto parser = LibSlg::Parser(tokens);
		EXPECT_NO_THROW(parser.parse());
		m_statements = parser.parse();
		m_jsonPrinter = new LibSlg::AstJsonPrinter(m_statements);
	}

	void TearDown() override {
		delete m_jsonPrinter;
	}
};

class ParserLiteral : public Parser {};

class ParserGroup : public Parser {};

class ParserPrecedence : public Parser {};

class ParserObject : public Parser {};

TEST_P(ParserLiteral, Literal) {
	EXPECT_THAT(m_jsonPrinter->dump(), MatchesRegex(
			R""({"type":"Expression","value":{"type":"Literal","literalType":"(STRING|BOOL|NUMBER|NOTHING)","value":".*"}})""));
}

TEST_P(ParserGroup, Grouping) {
	EXPECT_THAT(m_jsonPrinter->dump(), MatchesRegex(
			R""({"type":"Expression","value":{"type":"Group","value":{"type":"Literal","literalType":"NUMBER","value":".*"}}})""));
}

TEST_P(ParserPrecedence, Precedance) {
	EXPECT_STREQ(m_jsonPrinter->dump().c_str(),
			R""({"type":"Expression","value":{"type":"Binary","value":{"operator":"PLUS","lhs":{"type":"Literal","literalType":"NUMBER","value":"1"},"rhs":{"type":"Binary","value":{"operator":"STAR","lhs":{"type":"Literal","literalType":"NUMBER","value":"2"},"rhs":{"type":"Literal","literalType":"NUMBER","value":"3"}}}}}})"");
}

TEST_P(ParserObject, Object) {
	EXPECT_STREQ(m_jsonPrinter->dump().c_str(),
			R"({"type":"Declaration","value":{"variable":"class","newValue":{"type":"Object","value":{"implementation":{"type":"Block","value":{"implementation":[{"type":"Declaration","value":{"variable":"attribute","newValue":{"type":"Literal","literalType":"NUMBER","value":"6"}}},{"type":"Declaration","value":{"variable":"method","newValue":{"type":"Function","value":{"parameters":["a","b","c"], "implementation":{"type":"Block","value":{"implementation":[{"type":"Return","value":{"type":"Function","value":{"parameters":[], "implementation":{"type":"Block","value":{"implementation":[{"type":"Return","value":{"type":"Variable","value":"a"}}]}}}}}]}}}}}}]}}}}}})");
}

INSTANTIATE_TEST_SUITE_P(number, ParserLiteral, ::testing::Values("123;"));
INSTANTIATE_TEST_SUITE_P(boolean, ParserLiteral, ::testing::Values("true;", "false;"));
INSTANTIATE_TEST_SUITE_P(string, ParserLiteral, ::testing::Values("\"String\";", "\"Str\ning\";", "\"\";"));
INSTANTIATE_TEST_SUITE_P(nothing, ParserLiteral, ::testing::Values("nothing;"));

INSTANTIATE_TEST_SUITE_P(group, ParserGroup, ::testing::Values("(1);"));

INSTANTIATE_TEST_SUITE_P(precedance, ParserPrecedence, ::testing::Values("1+2*3;"));

INSTANTIATE_TEST_SUITE_P(object, ParserObject, ::testing::Values(R"(
var class = object {
	var attribute = 6;
	var method = fun (a, b, c){
		return fun (){
			return a;
		};
	};
};
)"));