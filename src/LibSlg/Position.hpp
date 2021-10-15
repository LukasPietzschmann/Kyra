#pragma once

#include <ostream>
#include <sstream>
#include <string>

namespace Slanguage {
struct Position {
	Position(unsigned int startLine, unsigned int startColumn, unsigned int endLine, unsigned int endColumn) :
		start(Start(startLine, startColumn)), end(End(endLine, endColumn)) {}
	Position(const Position& start, const Position& end) : start(start.start), end(end.end) {}

	struct Start {
		Start(unsigned int line, unsigned int column) : line(line), column(column) {}
		unsigned int line;
		unsigned int column;

		friend std::ostream& operator<<(std::ostream& os, const Position::Start& rhs) {
			os << rhs.column << ":" << rhs.line;
			return os;
		}
	};
	struct End {
		End(unsigned int line, unsigned int column) : line(line), column(column) {}
		unsigned int line;
		unsigned int column;

		friend std::ostream& operator<<(std::ostream& os, const Position::End& rhs) {
			os << rhs.column << ":" << rhs.line;
			return os;
		}
	};

	Start start;
	End end;

	bool operator==(const Position& rhs) const {
		return start.line == rhs.start.line && start.column == rhs.start.column && end.line == rhs.end.line &&
			   end.column == rhs.end.column;
	}
};
}