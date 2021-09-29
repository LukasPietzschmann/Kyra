#include "Lexer.hpp"

namespace LibSlg {
std::vector<Token> Lexer::scanTokens() {
	while(!isAtEnd()) {
		scanToken();
		m_startCharacter = m_currentCharacter;
		m_startLine = m_currentLine;
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
		case '\n':
			++m_currentLine;
			m_characterAtLineStart = m_currentCharacter;
			break;
		case '#': comment(); break;
		case '(': addToken(TokenType::LEFT_PAREN); break;
		case ')': addToken(TokenType::RIGHT_PAREN); break;
		case '{': addToken(TokenType::LEFT_CURLY); break;
		case '}': addToken(TokenType::RIGHT_CURLY); break;
		case ',': addToken(TokenType::COMMA); break;
		case ';': addToken(TokenType::SEMICOLON); break;
		case '.': addToken(TokenType::DOT); break;
		case ':': addToken(TokenType::COLON); break;
		case '+': addToken(TokenType::PLUS); break;
		case '/': addToken(TokenType::SLASH); break;
		case '*': addToken(TokenType::STAR); break;
		case '-': addToken(matchAndAdvance('>') ? TokenType::ARROW : TokenType::MINUS); break;
		case '=': addToken(matchAndAdvance('=') ? TokenType::EQUAL_EQUAL : TokenType::EQUAL); break;
		case '!': addToken(matchAndAdvance('=') ? TokenType::BANG_EQUAL : TokenType::BANG); break;
		case '>': addToken(matchAndAdvance('=') ? TokenType::GREATER_EQUAL : TokenType::GREATER); break;
		case '<': addToken(matchAndAdvance('=') ? TokenType::LESS_EQUAL : TokenType::LESS); break;
		case '"': string(); break;
		default:
			if(isDigit(current))
				number();
			else if(isAlpha(current))
				nameOrKeyword();
			else
				throw LexerException(Position(m_startLine, m_startCharacter, m_currentLine, m_currentCharacter),
						"Unexpected Character \"" + std::string(1, current) + "\" ");
	}
}

void Lexer::comment() {
	while(!match('\n') && !isAtEnd())
		advance();
}

void Lexer::number() {
	while(isDigit(peek()))
		advance();
	addToken(TokenType::NUMBER, m_source.substr(m_startCharacter, m_currentCharacter - m_startCharacter));
}

void Lexer::string() {
	while(!match('"') && !isAtEnd()) {
		if(match('\n'))
			++m_currentLine;
		advance();
	}

	if(isAtEnd())
		throw LexerException(Position(m_startLine, m_startCharacter, m_currentLine, m_currentCharacter),
				"Unterminated String " +
						m_source.substr(m_startCharacter + 1, m_currentCharacter - m_startCharacter - 1));

	advance();

	const std::string& string = m_source.substr(m_startCharacter + 1, m_currentCharacter - m_startCharacter - 2);
	addToken(TokenType::STRING, string);
}

void Lexer::nameOrKeyword() {
	while(isAlpha(peek()) || isDigit(peek()))
		advance();

	std::string string = m_source.substr(m_startCharacter, m_currentCharacter - m_startCharacter);
	std::string literal;
	TokenType type = TokenType::NAME;
	if(m_keywords.find(string) != m_keywords.end())
		type = m_keywords.at(string);
	else
		literal = m_source.substr(m_startCharacter, m_currentCharacter - m_startCharacter);

	addToken(type, literal);
}

char Lexer::advance() { return m_source[m_currentCharacter++]; }

char Lexer::peek() const {
	if(isAtEnd())
		return '\0';
	return m_source[m_currentCharacter];
}

bool Lexer::match(char expected) const { return peek() == expected; }

bool Lexer::matchAndAdvance(char expected) {
	if(!match(expected))
		return false;
	advance();
	return true;
}

void Lexer::addToken(TokenType type, const std::string& literal) {
	std::string lexeme;
	if(type != TokenType::END_OF_FILE)
		lexeme = m_source.substr(m_startCharacter, m_currentCharacter - m_startCharacter);
	int currentColumn = m_currentCharacter - m_characterAtLineStart + 1;
	int startColumn = m_startCharacter - m_characterAtLineStart + 1;
	m_tokens.emplace_back(type, Position(m_startLine, startColumn, m_currentLine, currentColumn), lexeme, literal);
}

bool Lexer::isDigit(char character) { return character >= '0' && character <= '9'; }

bool Lexer::isAlpha(char character) {
	return (character >= 'a' && character <= 'z') || (character >= 'A' && character <= 'Z');
}

bool Lexer::isAtEnd() const { return (unsigned long)m_currentCharacter >= m_source.size(); }
}