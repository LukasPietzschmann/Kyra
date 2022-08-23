#pragma once

class SourceRange {
public:
	struct Position {
		Position(unsigned line, unsigned index, unsigned line_start_index);

		unsigned line;
		// index of the character in the input string
		unsigned index;
		// index of the lines first character
		unsigned line_start_index;
	};

	SourceRange(const Position& start, const Position& end);

	const Position& get_start() const;
	const Position& get_end() const;

private:
	const Position m_start;
	const Position m_end;
};
