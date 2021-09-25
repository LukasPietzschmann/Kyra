#pragma once

#include <ostream>
#include <sstream>
#include <string>

namespace LibSlg {
struct Position {
	Position(unsigned int startLine, unsigned int startColumn, unsigned int endLine, unsigned int endColumn) :
		start(Start(startLine, startColumn)), end(End(endLine, endColumn)) {}

	struct Start {
		Start(unsigned int line, unsigned int column) : line(line), column(column) {}
		unsigned int line;
		unsigned int column;
	};
	struct End {
		End(unsigned int line, unsigned int column) : line(line), column(column) {}
		unsigned int line;
		unsigned int column;
	};

	Start start;
	End end;

	std::string toString() const {
		std::stringstream ss;
		ss << "from " << start.line << ":" << start.column << " to " << end.line << ":" << end.column;
		return ss.str();
	}

	bool operator==(const Position& rhs) const {
		return start.line == rhs.start.line && start.column == rhs.start.column && end.line == rhs.end.line &&
			   end.column == rhs.end.column;
	}
	bool operator!=(const Position& rhs) const { return !(rhs == *this); }

	friend std::ostream& operator<<(std::ostream& os, const Position& position) {
		os << position.toString();
		return os;
	}
};
}