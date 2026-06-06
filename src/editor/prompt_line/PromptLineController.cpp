#include <algorithm>
#include <utility>

#include "PromptLineController.hpp"

namespace Tedit {

void PromptLineController::activate() {
	reset_input();
	m_is_active = true;
}

void PromptLineController::deactivate() {
	reset_input();
	m_is_active = false;
}

bool PromptLineController::is_active() const {
	return m_is_active;
}

std::string_view PromptLineController::input() const {
	return m_input;
}

std::string_view PromptLineController::inactive_output() const {
	return m_inactive_output;
}

int PromptLineController::cursor_col() const {
	return m_cursor_col;
}

void PromptLineController::set_inactive_output(std::string output) {
	m_inactive_output = std::move(output);
}

void PromptLineController::insert_char(char c) {
	m_input.insert(m_input.begin() + (m_cursor_col - 1), c);
	m_cursor_col++;
}

void PromptLineController::backspace() {
	if (m_cursor_col <= 1)
		return;

	m_input.erase(m_input.begin() + (m_cursor_col - 2));
	m_cursor_col--;
}

void PromptLineController::move_left() {
	m_cursor_col = std::max(m_cursor_col - 1, 1);
}

void PromptLineController::move_right() {
	int last_valid_column { static_cast<int>(m_input.size()) + 1 };
	m_cursor_col = std::min(m_cursor_col + 1, last_valid_column);
}

void PromptLineController::reset_input() {
	m_input = "";
	m_cursor_col = 1;
}

}
