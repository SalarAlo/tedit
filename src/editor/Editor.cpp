#include <curses.h>

#include "Editor.h"

#include "InsertMode.h"

namespace Tedit {

Editor::Editor() {
	change_mode(std::make_unique<InsertMode>());
}

void Editor::handle_key(int key) {
	auto cmd { m_mode->map(key) };
	if (cmd)
		cmd->execute(*this);
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

void Editor::change_mode(std::unique_ptr<Mode> mode) {
	m_mode = std::move(mode);
}

bool Editor::should_close() const { return m_should_close; }

void Editor::close() { m_should_close = true; }

}
