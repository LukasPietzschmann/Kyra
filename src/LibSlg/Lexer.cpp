#include "Lexer.hpp"

namespace LibSlg {
std::vector<Token> Lexer::scanTokens() {
	while(!isAtEnd()) {
		scanToken();
		m_start = m_current;
	}
	addToken(TokenType::END_OF_FILE);
	return m_tokens;
}

void Lexer::scanToken() {
	char current = advance();
	switch(current) {
		case ' ':
		case '\t':
		case '\r': break;
		case '\n': ++m_line;
			break;
		case '#': comment();
			break;
		case '(': addToken(TokenType::LEFT_PAREN);
			break;
		case ')': addToken(TokenType::RIGHT_PAREN);
			break;
		case '{': addToken(TokenType::LEFT_CURLY);
			break;
		case '}': addToken(TokenType::RIGHT_CURLY);
			break;
		case ',': addToken(TokenType::COMMA);
			break;
		case ';': addToken(TokenType::SEMICOLON);
			break;
		case '.': addToken(TokenType::DOT);
			break;
		case '-': addToken(TokenType::MINUS);
			break;
		case '+': addToken(TokenType::PLUS);
			break;
		case '/': addToken(TokenType::SLASH);
			break;
		case '*': addToken(TokenType::STAR);
			break;
		case '=': addToken(matchAndAdvance('=') ? TokenType::EQUAL_EQUAL : TokenType::EQUAL);
			break;
		case '!': addToken(matchAndAdvance('=') ? TokenType::BANG_EQUAL : TokenType::BANG);
			break;
		case '>': addToken(matchAndAdvance('=') ? TokenType::GREATER_EQUAL : TokenType::GREATER);
			break;
		case '<': addToken(matchAndAdvance('=') ? TokenType::LESS_EQUAL : TokenType::LESS);
			break;
		case '"': string();
			break;
		default:
			if(isDigit(current))
				number();
			else if(isAlpha(current))
				nameOrKeyword();
			else
				std::cerr << "Unexpected Character \"" << current << "\" at Line " << m_line << std::endl;
	}
}

void Lexer::comment() {
	while(!match('\n') && !isAtEnd())
		advance();
}

void Lexer::number() {
	while(isDigit(peek()))
		advance();
	addToken(TokenType::NUMBER, m_source.substr(m_start, m_current - m_start));
}

void Lexer::string() {
	while(!match('"') && !isAtEnd()) {
		if(match('\n'))
			++m_line;
		advance();
	}

	if(isAtEnd()) {
		std::cerr << "Unterminated String" << std::endl;
		return;
	}

	advance();

	const std::string& string = m_source.substr(m_start + 1, m_current - m_start - 2);
	addToken(TokenType::STRING, string);
}

void Lexer::nameOrKeyword() {
	while(isAlpha(peek()) || isDigit(peek()))
		advance();

	std::string string = m_source.substr(m_start, m_current - m_start);
	std::string literal;
	TokenType type = TokenType::NAME;
	if(keywords.find(string) != keywords.end())
		type = keywords.at(string);
	else
		literal = m_source.substr(m_start, m_current - m_start);

	addToken(type, literal);
}

char Lexer::advance() {
	return m_source[m_current++];
}

char Lexer::peek() const {
	if(isAtEnd())
		return '\0';
	return m_source[m_current];
}

bool Lexer::match(char expected) const {
	return peek() == expected;
}

bool Lexer::matchAndAdvance(char expected) {
	if(!match(expected))
		return false;
	advance();
	return true;
}

void Lexer::addToken(TokenType type, const std::string& literal) {
	const std::string& lexeme = m_source.substr(m_start, m_current - m_start);
	m_tokens.emplace_back(type, m_line, lexeme, literal);
}

bool Lexer::isDigit(char character) {
	return character >= '0' && character <= '9';
}

bool Lexer::isAlpha(char character) {
	return (character >= 'a' && character <= 'z') || (character >= 'A' && character <= 'Z');
}

bool Lexer::isAtEnd() const {
	return (unsigned long) m_current >= m_source.size();
}
}