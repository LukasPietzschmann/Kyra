#pragma once

#include <ostream>
#include <sstream>
#include <string>

namespace Kyra {
struct Position {
	Position(unsigned int startLine, unsigned int startColumn, unsigned int endLine, unsigned int endColumn);
	Position(const Position& start, const Position& end);

	struct PositionPart {
		PositionPart(unsigned int line, unsigned int column);
		unsigned int line;
		unsigned int column;

		friend std::ostream& operator<<(std::ostream& os, const Position::PositionPart& part);
	};

	PositionPart start;
	PositionPart end;

	bool operator==(const Position& rhs) const;
};
}