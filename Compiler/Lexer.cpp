#include "Lexer.hpp"

#include <cassert>
#include <map>

const std::vector<Token>& Lexer::scan_input(std::string_view source) {
	m_source = source;
	m_tokens.clear();

	while(!is_at_end()) {
		m_start_character = m_current_character;
		scan_token();
	}
	add_token(TokenType::END_OF_FILE);

	return m_tokens;
}

void Lexer::scan_token() {
	const char current = advance();
	switch(current) {
		case ' ':
		case '\t':
		case '\n':
		case '\r': break;  // skip whitespace
		case '(': add_token(TokenType::LEFT_PAREN); break;
		case ')': add_token(TokenType::RIGHT_PAREN); break;
		case '{': add_token(TokenType::LEFT_CURLY); break;
		case '}': add_token(TokenType::RIGHT_CURLY); break;
		case ',': add_token(TokenType::COMMA); break;
		case ';': add_token(TokenType::SEMICOLON); break;
		case ':': add_token(TokenType::COLON); break;
		case '-': add_token(TokenType::MINUS); break;
		case '+': add_token(TokenType::PLUS); break;
		case '*': add_token(TokenType::STAR); break;
		case '/': add_token(TokenType::SLASH); break;
		case '=': add_token(TokenType::EQUAL); break;
		case '#': comment(); break;
		default:
			if(is_digit(current))
				number();
			else if(is_alpha(current))
				name_or_keyword();
			else
				assert(0);	// TODO: Error
	}
}

void Lexer::comment() {
	bool is_multiline = match_and_advance('#');
	while(is_multiline && (!is_at_end() && !match_multiple_and_advance("##")))
		advance();
	while(!is_multiline && (!is_at_end() && !match('\n')))
		advance();
}

void Lexer::number() {
	while(is_digit(peek()))
		advance();
	add_token(TokenType::NUMBER, m_source.substr(m_start_character, m_current_character - m_start_character));
}

void Lexer::name_or_keyword() {
	while(is_alpha(peek()) || is_digit(peek()))
		advance();
	std::string_view lexeme = m_source.substr(m_start_character, m_current_character - m_start_character);
	if(const auto& type_or_nil = is_keyword(lexeme); type_or_nil.has_value())
		add_token(*type_or_nil, lexeme);
	else
		add_token(TokenType::NAME, lexeme);
}

char Lexer::advance() {
	assert(!is_at_end());
	return m_source.at(m_current_character++);
}

char Lexer::peek() const {
	if(is_at_end())
		return '\0';
	return m_source.at(m_current_character);
}

bool Lexer::match(char expected) const { return peek() == expected; }

bool Lexer::match_and_advance(char expected) {
	if(!match(expected))
		return false;
	advance();
	return true;
}

std::string_view Lexer::peek_multiple(unsigned n) const {
	if(is_at_end())
		return "";
	return m_source.substr(m_current_character, n);
}

bool Lexer::match_multiple(std::string_view expected) const { return peek_multiple(expected.length()) == expected; }

bool Lexer::match_multiple_and_advance(std::string_view expected) {
	if(!match_multiple(expected))
		return false;
	for(auto i = 0; i < expected.length(); ++i)
		advance();
	return true;
}

bool Lexer::is_digit(char character) const { return character >= '0' && character <= '9'; }

bool Lexer::is_alpha(char character) const {
	return (character >= 'a' && character <= 'z') || (character >= 'A' && character <= 'Z');
}

std::optional<TokenType> Lexer::is_keyword(std::string_view string) const {
	static const std::map<std::string_view, TokenType> keywords{{"var", TokenType::VAR},
			{"val", TokenType::VAL},
			{"fun", TokenType::FUN},
			{"return", TokenType::RETURN}};

	if(const auto& it = keywords.find(string); it != keywords.end())
		return it->second;
	return {};
}

bool Lexer::is_at_end() const { return m_current_character >= m_source.length(); }

void Lexer::add_token(TokenType type, std::string_view literal) {
	std::string_view lexeme;
	if(type != TokenType::END_OF_FILE)
		lexeme = m_source.substr(m_start_character, m_current_character - m_start_character);
	m_tokens.emplace_back(type, lexeme, literal);
}
