#include "Position.hpp"

namespace Kyra {
Position::PositionPart::PositionPart(unsigned int line, unsigned int column) : line(line), column(column) {}

std::ostream& operator<<(std::ostream& os, const Position::PositionPart& part) {
	os << part.line << ":" << part.column;
	return os;
}

Position::Position(unsigned int startLine, unsigned int startColumn, unsigned int endLine, unsigned int endColumn) :
	start(PositionPart(startLine, startColumn)), end(PositionPart(endLine, endColumn)) {}

Position::Position(const Position& start, const Position& end) : start(start.start), end(end.end) {}

bool Position::operator==(const Position& rhs) const {
	return start.line == rhs.start.line && start.column == rhs.start.column && end.line == rhs.end.line &&
		   end.column == rhs.end.column;
}
}