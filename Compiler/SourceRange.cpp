#include "SourceRange.hpp"

SourceRange::Position::Position(unsigned line, unsigned index, unsigned line_start_index) :
	line(line), index(index), line_start_index(line_start_index) {}

SourceRange::SourceRange(const Position& start, const Position& end) : m_start(start), m_end(end) {}

const SourceRange::Position& SourceRange::get_start() const { return m_start; }

const SourceRange::Position& SourceRange::get_end() const { return m_end; }
