
#include "TextBuffer.hpp"

namespace Tedit {

TextBuffer::TextBuffer(std::unique_ptr<TextBufferSource> src)
    : m_source(std::move(src)) {
	set_text(m_source->read());

	if (m_lines.empty())
		m_lines.emplace_back("");
}

void TextBuffer::insert_char(int row, int col, char c) {
	if (row < 0 || row >= line_count())
		throw std::out_of_range("row");

	auto& ln = m_lines[row];

	if (col < 0 || col > static_cast<int>(ln.size()))
		throw std::out_of_range("col");

	ln.insert(ln.begin() + col, c);
}

void TextBuffer::erase_char(int row, int col) {
	if (row < 0 || row >= line_count())
		throw std::out_of_range("row");

	auto& ln = m_lines[row];

	if (col >= static_cast<int>(ln.size()))
		throw std::out_of_range("col");

	ln.erase(ln.begin() + col);
}

void TextBuffer::insert_newline(int row, int col) {
	if (row < 0 || row >= line_count())
		throw std::out_of_range("row");

	auto& ln = m_lines[row];

	if (col < 0 || col > static_cast<int>(ln.size()))
		throw std::out_of_range("col");

	std::string right = ln.substr(col);
	ln.erase(col);

	m_lines.insert(m_lines.begin() + row + 1, std::move(right));
}

std::string_view TextBuffer::line(int row) const {
	if (row < 0 || row >= line_count())
		throw std::out_of_range("row");

	return m_lines[row];
}

int TextBuffer::line_count() const {
	return static_cast<int>(m_lines.size());
}

void TextBuffer::erase_line(int row) {
	if (row < 0 || row >= line_count())
		throw std::out_of_range("row");

	m_lines.erase(m_lines.begin() + row);
}

void TextBuffer::append_to(int row, std::string_view txt) {
	if (row < 0 || row >= line_count())
		throw std::out_of_range("row");
	m_lines[row].append(txt);
}

void TextBuffer::set_text(std::string_view txt) {
	m_lines.clear();
	std::string accumulator {};

	for (size_t i {}; i < txt.size(); i++) {
		if (txt[i] == '\n') {
			m_lines.push_back(std::move(accumulator));
			accumulator.clear();
			continue;
		}

		accumulator += txt[i];
	}

	if (!accumulator.empty())
		m_lines.push_back(std::move(accumulator));
}

void TextBuffer::save() {
	std::string output {};

	for (const auto& line : m_lines) {
		output += line + '\n';
	}

	m_source->write(output);
}

std::string TextBuffer::get_source_name() { return m_source->get_buffer_name(); }

}
