#include "Error.hpp"

#include <filesystem>
#include <fstream>
#include <map>
#include <string>

ErrorException::ErrorException(std::string_view message, const SourceRange& source_range) :
	m_message(message), m_source_range(source_range) {}

const char* ErrorException::what() const noexcept { return m_message.data(); }

Error::Error(const ErrorException& exception) : m_exception(exception) {}

void Error::print(std::ostream& stream) const {
	stream << m_exception.what() << std::endl;
	const SourceRange& sr = m_exception.m_source_range;
	if(sr.get_start().line - sr.get_end().line == 0) {
		static std::map<std::filesystem::path, std::string> file_content_cache;

		std::string file_content;
		if(const auto& it = file_content_cache.find(sr.get_file_path()); it != file_content_cache.end())
			file_content = it->second;
		else {
			std::ifstream input(sr.get_file_path());
			file_content = std::string((std::istreambuf_iterator<char>(input)), (std::istreambuf_iterator<char>()));
			file_content_cache.try_emplace(sr.get_file_path(), file_content);
		}
		std::string span = file_content.substr(sr.get_start().index, sr.get_end().index - sr.get_start().index);
		stream << span << std::endl;
	}
}
