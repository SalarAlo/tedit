#include <algorithm>
#include <utility>

#include "CommandLineController.hpp"

namespace Tedit {

void CommandLineController::activate() {
	reset_input();
	m_is_active = true;
}

void CommandLineController::deactivate() {
	reset_input();
	m_is_active = false;
}

bool CommandLineController::is_active() const {
	return m_is_active;
}

std::string_view CommandLineController::command() const {
	return m_command;
}

std::string_view CommandLineController::inactive_output() const {
	return m_inactive_output;
}

int CommandLineController::cursor_col() const {
	return m_cursor_col;
}

void CommandLineController::set_inactive_output(std::string output) {
	m_inactive_output = std::move(output);
}

void CommandLineController::insert_char(char c) {
	m_command.insert(m_command.begin() + (m_cursor_col - 1), c);
	m_cursor_col++;
}

void CommandLineController::backspace() {
	if (m_cursor_col <= 1)
		return;

	m_command.erase(m_command.begin() + (m_cursor_col - 2));
	m_cursor_col--;
}

void CommandLineController::move_left() {
	m_cursor_col = std::max(m_cursor_col - 1, 1);
}

void CommandLineController::move_right() {
	int last_valid_column { static_cast<int>(m_command.size()) + 1 };
	m_cursor_col = std::min(m_cursor_col + 1, last_valid_column);
}

void CommandLineController::reset_input() {
	m_command = "";
	m_cursor_col = 1;
}

}
