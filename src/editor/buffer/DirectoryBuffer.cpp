#include "DirectoryBuffer.hpp"

#include "Editor.hpp"

namespace Tedit {

DirectoryBuffer::DirectoryBuffer(const fs::path& directory)
    : m_directory(directory) {
	apply_directory();
}

std::string_view DirectoryBuffer::line(int row) const {
	if (row < 0 || row >= line_count())
		throw std::out_of_range("row");

	return m_lines[row].display_name;
}

int DirectoryBuffer::line_count() const { return m_lines.size(); }

std::string DirectoryBuffer::get_name() const { return m_directory.filename(); }

void DirectoryBuffer::apply_directory() {
	if (!fs::is_directory(m_directory)) {
		throw std::logic_error(std::format("can't apply non directory path for directory buffer \"{}\"", m_directory.string()));
	}

	m_lines.clear();

	m_lines.emplace_back("../", m_directory.parent_path());

	for (const fs::path& child : fs::directory_iterator(m_directory)) {
		auto display_name { child.filename().string() + (fs::is_directory(child) ? "/" : "") };
		m_lines.emplace_back(display_name, child);
	}
}

void DirectoryBuffer::select(Editor& editor) {
	const auto& path { m_lines[m_cursor.row].file_path };
	m_directory = path;

	m_cursor.reset();

	if (!fs::is_directory(m_directory)) {
		editor.open_path(path);
		return;
	}

	apply_directory();
}

}
