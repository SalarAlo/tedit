#include <fstream>
#include <stdexcept>

#include "FileBufferSource.h"

namespace Tedit {
FileBufferSource::FileBufferSource(const fs::path& path)
    : m_path(path) { }

void FileBufferSource::write(std::string_view txt) {
	std::ofstream file { m_path, std::ios::binary | std::ios::trunc };
	if (!file.is_open()) {
		throw std::runtime_error("Invalid file supplied into file buffer source.");
	}

	file.write(txt.data(), txt.size());

	if (!file) {
		throw std::runtime_error("Failed while writing to file.");
	}
}

std::string FileBufferSource::read() {
	std::fstream file { m_path };
	if (!file.is_open()) {
		throw std::runtime_error("Invalid file supplied into file buffer source.");
	}
	std::string line {};
	std::string contents {};

	while (std::getline(file, line))
		contents += line + '\n';

	return contents;
}

std::string FileBufferSource::get_display_name() const { return m_path.stem(); }

}
