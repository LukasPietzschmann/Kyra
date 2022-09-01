#include "Error.hpp"

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <map>
#include <string>

namespace Kyra {

ErrorException::ErrorException(std::string_view message, const SourceRange& source_range) :
	m_message(message), m_source_range(source_range) {}

const char* ErrorException::what() const noexcept { return m_message.data(); }

const SourceRange& ErrorException::get_source_range() const { return m_source_range; }

void ErrorException::print(std::ostream& stream) const {
	stream << m_message << '\n';
	const SourceRange& source_range = m_source_range;
	if(source_range.get_start().line - source_range.get_end().line == 0) {
		static std::map<std::filesystem::path, std::string> file_content_cache;

		const std::filesystem::path& file_path = source_range.get_file_path();

		std::string file_content;
		if(const auto& it = file_content_cache.find(file_path); it != file_content_cache.end())
			file_content = it->second;
		else {
			std::ifstream input(file_path);
			file_content = std::string((std::istreambuf_iterator<char>(input)), (std::istreambuf_iterator<char>()));
			file_content_cache.try_emplace(file_path, file_content);
			input.close();
		}

		static const unsigned margin = 10;
		const unsigned start_index = source_range.get_start().index;
		const unsigned end_index = source_range.get_end().index;
		const unsigned lower_index = start_index -
			std::min(std::max(margin, margin / 2), start_index - source_range.get_start().line_start_index);
		const unsigned upper_index = std::min<unsigned>(file_content.find('\n', end_index), end_index + margin);
		std::string span = file_content.substr(lower_index, upper_index - lower_index);

		// Replace tabs with four spaces
		unsigned replacements = 0;
		for(size_t n = 0; (n = span.find('\t', n)) != std::string::npos; ++replacements)
			span.replace(n, 1, "    ");

		std::string line_number = std::to_string(source_range.get_start().line) + ": ";
		std::string underline(end_index - start_index, '~');
		std::string padding(start_index - lower_index + line_number.length() + replacements * 3, ' ');
		stream << line_number << span << '\n';
		stream << padding << "\033[31m" << underline << "\033[0m\n";
	}
	stream.flush();
}
}
