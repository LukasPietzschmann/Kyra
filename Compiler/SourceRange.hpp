#pragma once

#include <cassert>
#include <filesystem>

namespace Kyra {

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

	SourceRange(const Position& start, const Position& end, const std::filesystem::path& file_path);

	static SourceRange unite(const SourceRange& start, const SourceRange& end) {
		assert(start.m_file_path == end.m_file_path);
		return SourceRange(start.m_start, end.m_end, start.m_file_path);
	}

	const Position& get_start() const;
	const Position& get_end() const;
	const std::filesystem::path& get_file_path() const;

private:
	const Position m_start;
	const Position m_end;
	const std::filesystem::path& m_file_path;
};
}
