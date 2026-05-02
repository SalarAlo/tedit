#include <curses.h>

#include "Editor.h"

#include "NormalMode.h"

namespace Tedit {

Editor::Editor() {
	change_mode(std::make_unique<NormalMode>());
}

void Editor::handle_key(int key) {
	auto action { m_mode->map_action(key) };

	if (action)
		action->execute(*this);
}

void Editor::backspace() {
	if (m_cmd_line.is_active) {
		if (m_cmd_line.cursor_col <= 1)
			return;

		m_cmd_line.command.erase(m_cmd_line.command.begin() + (m_cmd_line.cursor_col - 2));
		m_cmd_line.cursor_col--;
	} else {
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
}

void Editor::delete_char() {
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
	move_right();
}

void Editor::newline() {
	if (m_cmd_line.is_active)
		return;

	m_buffer.insert_newline(m_cursor.row, m_cursor.col);
	m_cursor.col = 0;
	move_down();
}

void Editor::insert_char(char c) {
	if (m_cmd_line.is_active) {
		m_cmd_line.command.insert(m_cmd_line.command.begin() + (m_cmd_line.cursor_col - 1), c);
		m_cmd_line.cursor_col++;
	} else {
		m_buffer.insert_char(m_cursor.row, m_cursor.col, c);
		move_right();
	}
}

void Editor::move_left() {
	if (m_cmd_line.is_active) {
		m_cmd_line.cursor_col = std::max(m_cmd_line.cursor_col - 1, 1);
		return;
	}

	m_cursor.col = std::max(m_cursor.col - 1, 0);
}

void Editor::move_right() {
	if (m_cmd_line.is_active) {
		int last_valid_column { static_cast<int>(m_cmd_line.command.size()) + 1 };
		m_cmd_line.cursor_col = std::min(m_cmd_line.cursor_col + 1, last_valid_column);
		return;
	}

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

void Editor::save_to_buffer() {
	m_buffer.save();
}

Cursor& Editor::get_cursor() { return m_cursor; }

void Editor::activate_command_line() {
	m_cmd_line.command = "";
	m_cmd_line.cursor_col = 1;
	m_cmd_line.is_active = true;
}

void Editor::deactivate_command_line() {
	m_cmd_line.command = "";
	m_cmd_line.cursor_col = 1;
	m_cmd_line.is_active = false;
}

void Editor::parse_and_leave_cmd_line() {
	auto result { CommandLineParser::parse(m_cmd_line.command) };
	deactivate_command_line();

	if (result.has_value()) {
		switch (*result) {
		case CommandType::Write:
			save_to_buffer();
			break;
		case CommandType::Quit:
			m_should_close = true;
			break;
		}
	}

	m_cmd_line.inactive_output = result.error_or("");
}

void Editor::move_start_line() { m_cursor.col = 0; }

}
