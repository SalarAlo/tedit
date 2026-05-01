#include <algorithm>

#include "TextBuffer.h"

namespace Tedit {

TextBuffer::TextBuffer(std::unique_ptr<TextBufferSource> source)
    : m_source(std::move(source)) {
	set_text(read_from_source());

	if (m_lines.empty())
		m_lines.emplace_back("");
}

void TextBuffer::write_into_source() {
	std::string output {};

	for (size_t i = 0; i < m_lines.size(); i++) {
		output += m_lines[i];
		output += '\n';
	}

	m_source->write(output);
}

std::string TextBuffer::read_from_source() {
	return m_source->read();
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

}
