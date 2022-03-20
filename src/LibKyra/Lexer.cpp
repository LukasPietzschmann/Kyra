#include "Lexer.hpp"

#include <cassert>
#include <cctype>
#include <utility>

#include "Exceptions.hpp"
#include "Position.hpp"

#define ADJUSTED_START_CHARACTER (m_start_position.column + m_character_at_start_line_start)
#define START_CURRENT_CHARACTER_DELTA (m_current_character - ADJUSTED_START_CHARACTER)
#define CURRENT_POSITION \
	{ m_current_line, m_current_character - m_character_at_current_line_start }

// FIXME: If the column gets printed, it should start at 1, not 0
namespace Kyra {
const Result<std::vector<Token>>& Lexer::scan_tokens() {
	m_result.reset();
	m_result.construct_value();
	try {
		while(!is_at_end()) {
			scan_token();
			m_character_at_start_line_start = m_character_at_current_line_start;
			m_start_position = {m_current_line, m_current_character - m_character_at_current_line_start};
		}
		add_token(TokenType::END_OF_FILE);
	} catch(const LexerException& e) { m_result.insert_error({e.what(), e.get_position()}); }
	return m_result;
}

void Lexer::scan_token() {
	char current = advance();
	switch(current) {
		case ' ':
		case '\t':
		case '\r': break;
		case '\n':
			++m_current_line;
			m_character_at_current_line_start = m_current_character;
			break;
		case '#': comment(); break;
		case '(': add_token(TokenType::LEFT_PAREN); break;
		case ')': add_token(TokenType::RIGHT_PAREN); break;
		case '{': add_token(TokenType::LEFT_CURLY); break;
		case '}': add_token(TokenType::RIGHT_CURLY); break;
		case ',': add_token(TokenType::COMMA); break;
		case ';': add_token(TokenType::SEMICOLON); break;
		case '.': add_token(TokenType::DOT); break;
		case ':': add_token(TokenType::COLON); break;
		case '+': add_token(TokenType::PLUS); break;
		case '/': add_token(TokenType::SLASH); break;
		case '*': add_token(TokenType::STAR); break;
		case '-': add_token(match_and_advance('>') ? TokenType::ARROW : TokenType::MINUS); break;
		case '=': add_token(match_and_advance('=') ? TokenType::EQUAL_EQUAL : TokenType::EQUAL); break;
		case '!': add_token(match_and_advance('=') ? TokenType::BANG_EQUAL : TokenType::BANG); break;
		case '>': add_token(match_and_advance('=') ? TokenType::GREATER_EQUAL : TokenType::GREATER); break;
		case '<': add_token(match_and_advance('=') ? TokenType::LESS_EQUAL : TokenType::LESS); break;
		case '"': string(); break;
		default:
			if(is_digit(current))
				number();
			else if(is_alpha(current))
				name_or_keyword();
			else
				throw LexerException(Position(m_start_position, CURRENT_POSITION),
						"Unexpected Character \"" + std::string(1, current) + "\" ");
	}
}

void Lexer::comment() {
	bool multiline = false;
	if(match_and_advance('#'))
		multiline = true;

	while(multiline && (!is_at_end() && !match_multiple_and_advance("##"))) {
		if(match('\n')) {
			m_character_at_current_line_start = m_current_character;
			++m_current_line;
		}
		advance();
	}
	while(!multiline && (!is_at_end() && !match('\n')))
		advance();
}

void Lexer::number() {
	while(is_digit(peek()))
		advance();
	add_token(TokenType::NUMBER, m_source.substr(ADJUSTED_START_CHARACTER, START_CURRENT_CHARACTER_DELTA));
}

void Lexer::string() {
	while(!match('"') && !is_at_end()) {
		if(match('\n')) {
			m_character_at_current_line_start = m_current_character;
			++m_current_line;
		}
		advance();
	}

	if(is_at_end())
		throw LexerException(Position(m_start_position, CURRENT_POSITION), "Unterminated String");

	advance();

	const std::string& string = m_source.substr(ADJUSTED_START_CHARACTER + 1, START_CURRENT_CHARACTER_DELTA - 2);
	add_token(TokenType::STRING, string);
}

void Lexer::name_or_keyword() {
	while(is_alpha(peek()) || is_digit(peek()))
		advance();

	std::string string = m_source.substr(ADJUSTED_START_CHARACTER, START_CURRENT_CHARACTER_DELTA);
	if(string == "operator") {
		while(!is_whitespace(peek()))
			advance();
	}
	string = m_source.substr(ADJUSTED_START_CHARACTER, START_CURRENT_CHARACTER_DELTA);
	std::string literal;
	TokenType type = TokenType::NAME;
	if(const auto& it = m_keywords.find(string); it != m_keywords.end())
		type = it->second;
	else
		literal = m_source.substr(ADJUSTED_START_CHARACTER, START_CURRENT_CHARACTER_DELTA);

	add_token(type, literal);
}

char Lexer::advance() { return m_source[m_current_character++]; }

char Lexer::peek() const {
	if(is_at_end())
		return '\0';
	return m_source[m_current_character];
}

std::string Lexer::peek_multiple(int n) const {
	assert(n > 0);
	if(is_at_end())
		return "";
	return m_source.substr(m_current_character, n);
}

bool Lexer::match(char expected) const { return peek() == expected; }

bool Lexer::match_and_advance(char expected) {
	if(!match(expected))
		return false;
	advance();
	return true;
}

bool Lexer::match_multiple(const std::string& expected) const {
	return peek_multiple((int)expected.size()) == expected;
}

bool Lexer::match_multiple_and_advance(const std::string& expected) {
	if(!match_multiple(expected))
		return false;
	for(unsigned long i = 0; i < expected.size(); ++i)
		advance();
	return true;
}

void Lexer::add_token(TokenType type, const std::string& literal) {
	std::string lexeme;
	if(type != TokenType::END_OF_FILE)
		lexeme = m_source.substr(ADJUSTED_START_CHARACTER, START_CURRENT_CHARACTER_DELTA);
	m_result.assert_get_value().emplace_back(type, Position(m_start_position, CURRENT_POSITION), lexeme, literal);
}

bool Lexer::is_digit(char character) { return character >= '0' && character <= '9'; }

bool Lexer::is_alpha(char character) {
	return (character >= 'a' && character <= 'z') || (character >= 'A' && character <= 'Z');
}

bool Lexer::is_whitespace(char character) { return isspace(character); }

bool Lexer::is_at_end() const { return (unsigned long)m_current_character >= m_source.size(); }
}

#undef ADJUSTED_START_CHARACTER
#undef START_CURRENT_CHARACTER_DELTA
#undef CURRENT_POSITION