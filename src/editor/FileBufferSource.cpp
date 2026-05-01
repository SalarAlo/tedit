#include <fstream>
#include <stdexcept>

#include "FileBufferSource.h"

namespace Tedit {

FileBufferSource::FileBufferSource(const fs::path& path)
    : m_file(path) { }

void FileBufferSource::write(std::string_view txt) {
	std::ofstream file { m_file, std::ios::trunc | std::ios::binary };

	if (!file.is_open())
		throw std::runtime_error("failed to open file.");

	file.write(txt.data(), txt.size());
}

std::string FileBufferSource::read() {
	std::ifstream file { m_file, std::ios::binary };

	if (!file.is_open())
		throw std::runtime_error("failed to open file.");

	return std::string(
	    std::istreambuf_iterator<char>(file),
	    std::istreambuf_iterator<char>());
}

}
