#include "Editor.h"

#include "InsertMode.h"

namespace Tedit {

Editor::Editor() {
	change_mode(std::make_unique<InsertMode>());
}

std::string Editor::format_line_number(int line, int width) {
	std::ostringstream oss;
	oss << std::setw(width) << line;
	return oss.str();
}

void Editor::poll_events() {
	m_last_key = m_terminal.read_key();
}

void Editor::draw() {
	m_terminal.clear();

	size_t gutter_width = std::to_string(m_buffer.line_count()).size();

	draw_gutter(gutter_width, true);
	draw_text(gutter_width);

	int offset = gutter_width + m_indent + 1;
	m_terminal.move_cursor(m_cursor.row, m_cursor.col + offset);

	m_terminal.present();
}

void Editor::draw_gutter(size_t gutter_width, bool relative) {
	for (int i = 0; i < m_buffer.line_count(); i++) {
		bool is_current_line { i == m_cursor.row };
		int line { relative ? make_relative(i) : i + 1 };

		std::string number = format_line_number(
		    line,
		    gutter_width + (is_current_line ? 0 : m_indent));

		m_terminal.draw_text(i, 0, number);
	}
}

void Editor::draw_text(size_t gutter_width) {
	int offset = gutter_width + 1;

	for (int i = 0; i < m_buffer.line_count(); i++) {
		std::string text = std::string(m_buffer.line(i));

		m_terminal.draw_text(i, offset + m_indent, text);
	}
}

bool Editor::should_close() const {
	return m_last_key == 'q' || m_last_key == 'Q';
}

void Editor::update() {
	m_mode->map(m_last_key)->execute(*this);
}

void Editor::backspace() {
	if (m_cursor.is_at_beginning())
		return;

	if (m_cursor.col == 0) {
		auto deleted_row { m_buffer.line(m_cursor.row) };
		m_buffer.erase_line(m_cursor.row);
		move_up();
		m_buffer.append_to(m_cursor.row, deleted_row);
		move_end_line();

		return;
	}
	move_left();
	m_buffer.erase_char(m_cursor.row, m_cursor.col);
}

void Editor::newline() {
	m_buffer.insert_newline(m_cursor.row, m_cursor.col);
	m_cursor.col = 0;
	move_down();
}

void Editor::insert_char(char c) {
	m_buffer.insert_char(m_cursor.row, m_cursor.col, c);
	move_right();
}

void Editor::move_left() {
	m_cursor.col = std::max(m_cursor.col - 1, 0);
}

void Editor::move_right() {
	int last_valid_column { static_cast<int>(current_line().size()) };
	m_cursor.col = std::min(m_cursor.col + 1, last_valid_column);
}

void Editor::move_up() {
	m_cursor.row = std::max(m_cursor.row - 1, 0);
	m_cursor.col = std::min(m_cursor.col, static_cast<int>(current_line().size()));
}

void Editor::move_down() {
	int last_valid_index { static_cast<int>(m_buffer.line_count() - 1) };
	m_cursor.row = std::min(m_cursor.row + 1, last_valid_index);
	m_cursor.col = std::min(m_cursor.col, static_cast<int>(current_line().size()));
}

std::string Editor::current_line() const {
	return std::string(m_buffer.line(m_cursor.row));
}

void Editor::move_end_line() {
	m_cursor.col = current_line().size();
}

int Editor::make_relative(int line) {
	if (m_cursor.row == line)
		return line + 1;

	return std::abs(m_cursor.row - line);
}

void Editor::change_mode(std::unique_ptr<Mode> mode) {
	m_mode.release();
	m_mode = std::move(mode);
}

}
