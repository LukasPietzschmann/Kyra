#include "Lexer.hpp"

#include <cassert>
#include <map>

namespace Kyra {

ErrorOr<std::vector<Token>> Lexer::scan_input(std::string_view source, const std::filesystem::path& file_path) {
	m_file_path = file_path;
	m_source = source;
	m_tokens.clear();

	try {
		while(!is_at_end()) {
			m_start = m_current;
			scan_token();
		}
	} catch(const ErrorException& e) {
		return e;
	}
	add_token(TokenType::END_OF_FILE);

	return m_tokens;
}

void Lexer::scan_token() {
	const char current = advance();
	switch(current) {
		case ' ':
		case '\t':
		case '\r': break; // skip whitespace
		case '\n': line_brak(); break;
		case '(': add_token(TokenType::LEFT_PAREN); break;
		case ')': add_token(TokenType::RIGHT_PAREN); break;
		case '{': add_token(TokenType::LEFT_CURLY); break;
		case '}': add_token(TokenType::RIGHT_CURLY); break;
		case '.': add_token(TokenType::DOT); break;
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
			else {
				SourceRange::Position start_copy = m_start;
				SourceRange::Position end_copy = m_current;
				throw ErrorException("Unknown character", SourceRange(start_copy, end_copy, m_file_path));
			}
	}
}

void Lexer::comment() {
	bool is_multiline = match_and_advance('#');
	while(is_multiline && (!is_at_end() && !match_multiple_and_advance("##"))) {
		if(match('\n'))
			line_brak();
		advance();
	}
	while(!is_multiline && (!is_at_end() && !match('\n')))
		advance();
}

void Lexer::number() {
	while(is_digit(peek()))
		advance();
	add_token(TokenType::NUMBER, m_source.substr(m_start.index, m_current.index - m_start.index));
}

void Lexer::name_or_keyword() {
	while(is_alpha(peek()) || is_digit(peek()) || is_overridable_operator(peek()))
		advance();
	std::string_view lexeme = m_source.substr(m_start.index, m_current.index - m_start.index);
	if(const auto& type_or_nil = is_keyword(lexeme); type_or_nil.has_value())
		add_token(*type_or_nil, lexeme);
	else
		add_token(TokenType::NAME, lexeme);
}

char Lexer::advance() {
	assert(!is_at_end());
	return m_source.at(m_current.index++);
}

char Lexer::peek() const {
	if(is_at_end())
		return '\0';
	return m_source.at(m_current.index);
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
	return m_source.substr(m_current.index, n);
}

bool Lexer::match_multiple(std::string_view expected) const { return peek_multiple(expected.length()) == expected; }

bool Lexer::match_multiple_and_advance(std::string_view expected) {
	if(!match_multiple(expected))
		return false;
	for(unsigned i = 0; i < expected.length(); ++i)
		advance();
	return true;
}

bool Lexer::is_digit(char character) const { return character >= '0' && character <= '9'; }

bool Lexer::is_alpha(char character) const {
	return (character >= 'a' && character <= 'z') || (character >= 'A' && character <= 'Z');
}

bool Lexer::is_overridable_operator(char character) const {
	switch(character) {
		case '+':
		case '-':
		case '*':
		case '/': return true;
		default: return false;
	}
}

std::optional<TokenType> Lexer::is_keyword(std::string_view string) const {
	static const std::map<std::string_view, TokenType> keywords{{"var", TokenType::VAR}, {"val", TokenType::VAL},
		{"struct", TokenType::STRUCT}, {"fun", TokenType::FUN}, {"print", TokenType::PRINT},
		{"return", TokenType::RETURN}};

	if(const auto& it = keywords.find(string); it != keywords.end())
		return it->second;
	return {};
}

bool Lexer::is_at_end() const { return m_current.index >= m_source.length(); }

void Lexer::add_token(TokenType type, std::string_view literal) {
	std::string_view lexeme;
	if(type != TokenType::END_OF_FILE)
		lexeme = m_source.substr(m_start.index, m_current.index - m_start.index);
	SourceRange::Position start_copy = m_start;
	SourceRange::Position end_copy = m_current;
	m_tokens.emplace_back(type, lexeme, literal, SourceRange(start_copy, end_copy, m_file_path));
}

void Lexer::line_brak() {
	++m_current.line;
	m_current.line_start_index = m_current.index;
}
}
