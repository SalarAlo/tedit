#include <variant>

#include "TextBuffer.hpp"

namespace Tedit {

TextBuffer::TextBuffer(std::unique_ptr<IBufferSource> src)
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
	m_revisions++;

	ln.insert(ln.begin() + col, c);
}

void TextBuffer::erase_char(int row, int col) {
	if (row < 0 || row >= line_count())
		throw std::out_of_range("row");

	auto& ln = m_lines[row];

	if (col >= static_cast<int>(ln.size()))
		throw std::out_of_range("col");

	ln.erase(ln.begin() + col);
	m_revisions++;
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

	m_revisions++;
}

std::string_view TextBuffer::line(int row) const {
	if (row < 0 || row >= line_count())
		throw std::out_of_range("row");

	return m_lines[row];
}

int TextBuffer::line_count() const {
	return static_cast<int>(m_lines.size());
}

std::string TextBuffer::text() const {
	std::string output {};

	for (const auto& line : m_lines) {
		output += line;
		output += '\n';
	}

	return output;
}

void TextBuffer::erase_line(int row) {
	if (row < 0 || row >= line_count())
		throw std::out_of_range("row");

	m_lines.erase(m_lines.begin() + row);
	m_revisions++;
}

void TextBuffer::append_to(int row, std::string_view txt) {
	if (row < 0 || row >= line_count())
		throw std::out_of_range("row");

	m_lines[row].append(txt);
	m_revisions++;
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

	m_revisions++;
}

void TextBuffer::save() {
	std::string output {};

	for (const auto& line : m_lines) {
		output += line + '\n';
	}

	m_source->write(output);
}

std::string TextBuffer::get_name() const {
	return m_source->get_buffer_name();
}

void TextBuffer::handle_undo(const HistoryAction& action) {
	std::visit([this](const auto& specific_action) { handle_undo(specific_action); }, action);
}

void TextBuffer::handle_undo(const InsertAction& insert_action) {
	erase_text_at(insert_action.before, insert_action.text);
	set_cursor(insert_action.before);
}

void TextBuffer::handle_undo(const DeleteAction& delete_action) {
	insert_text_at(delete_action.after, delete_action.text);
	set_cursor(delete_action.before);
}

void TextBuffer::insert_text_at(const Cursor& position, std::string_view text) {
	Cursor cursor { position };

	for (const char c : text) {
		if (c == '\n') {
			insert_newline(cursor.row, cursor.col);
			cursor.row++;
			cursor.col = 0;
			continue;
		}

		insert_char(cursor.row, cursor.col, c);
		cursor.col++;
	}
}

void TextBuffer::erase_text_at(const Cursor& position, std::string_view text) {
	Cursor cursor { position };

	for (const char c : text) {
		if (c == '\n') {
			if (cursor.row >= line_count() - 1)
				throw std::out_of_range("row");

			auto next_line { line(cursor.row + 1) };
			append_to(cursor.row, next_line);
			erase_line(cursor.row + 1);
			continue;
		}

		erase_char(cursor.row, cursor.col);
	}
}

const IBufferSource* TextBuffer::get_source() const { return m_source.get(); }

uint64_t TextBuffer::get_revisions() const { return m_revisions; }

}
