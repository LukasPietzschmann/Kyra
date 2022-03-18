#include "Position.hpp"

namespace Kyra {
Position::PositionPart::PositionPart(unsigned int line, unsigned int column) : line(line), column(column) {}

std::ostream& operator<<(std::ostream& os, const Position::PositionPart& part) {
	os << part.line << ":" << part.column;
	return os;
}

Position::Position(unsigned int start_line, unsigned int start_column, unsigned int end_line, unsigned int end_column) :
	start(PositionPart(start_line, start_column)), end(PositionPart(end_line, end_column)) {}

Position::Position(const Position::PositionPart& start, const Position::PositionPart& end) : start(start), end(end) {}

Position::Position(const Position& start, const Position& end) : Position(start.start, end.end) {}

bool Position::operator==(const Position& rhs) const {
	return start.line == rhs.start.line && start.column == rhs.start.column && end.line == rhs.end.line &&
		   end.column == rhs.end.column;
}

std::ostream& operator<<(std::ostream& os, const Position& position) {
	os << position.start << " - " << position.end;
	return os;
}
}