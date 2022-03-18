#pragma once

#include <ostream>

namespace Kyra {
struct Position {
	Position(unsigned int start_line, unsigned int start_column, unsigned int end_line, unsigned int end_column);
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
	friend std::ostream& operator<<(std::ostream& os, const Position& position);
};
}