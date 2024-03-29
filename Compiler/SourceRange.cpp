#include "SourceRange.hpp"

namespace Kyra {

SourceRange::Position::Position(unsigned line, unsigned index, unsigned line_start_index) :
	line(line), index(index), line_start_index(line_start_index) {}

SourceRange::SourceRange(const Position& start, const Position& end, const std::filesystem::path& file_path) :
	m_start(start), m_end(end), m_file_path(file_path) {}

const SourceRange::Position& SourceRange::get_start() const { return m_start; }

const SourceRange::Position& SourceRange::get_end() const { return m_end; }

const std::filesystem::path& SourceRange::get_file_path() const { return m_file_path; }
}
