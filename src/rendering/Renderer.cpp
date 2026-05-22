#include <algorithm>

#include "Renderer.hpp"

#include "Terminal.hpp"

namespace Tedit {

void Renderer::render(Editor& editor) {
	m_editor = &editor;

	Terminal::get_instance().clear();

	auto length { std::to_string(m_editor->m_buffer->line_count()).size() };
	size_t gutter_width { std::max<size_t>(3, length) };

	draw_gutter(gutter_width, true);
	draw_text(gutter_width);
	draw_bar_below();
	draw_cmd_line();

	if (m_editor->m_cmd_line.is_active) {
		auto [_, height] = Terminal::get_instance().get_terminal_dimensions();
		Terminal::get_instance().move_cursor(height - 1, m_editor->m_cmd_line.cursor_col);
	} else {
		size_t offset { gutter_width + INDENT + 1 };
		int screen_row { m_editor->m_cursor.row - static_cast<int>(m_editor->m_top_row) };
		Terminal::get_instance().move_cursor(screen_row, m_editor->m_cursor.col + static_cast<int>(offset));
	}

	Terminal::get_instance().present();
}

void Renderer::draw_gutter(size_t gutter_width, bool relative) {
	int max_lines { std::min<int>(m_editor->m_top_row + Terminal::get_instance().get_height() - BELOW_HEIGHT, m_editor->m_buffer->line_count()) };

	for (int i = m_editor->m_top_row; i < max_lines; i++) {
		bool is_current_line { i == m_editor->m_cursor.row };
		int line { relative ? make_relative(i) : i + 1 };
		int screen_row { i - static_cast<int>(m_editor->m_top_row) };

		std::string number = format_line_number(
		    line,
		    gutter_width + (is_current_line ? 0 : INDENT));

		Terminal::get_instance().draw_text(screen_row, 0, number);
	}
}

void Renderer::draw_text(size_t gutter_width) {
	int offset = gutter_width + 1;
	int max_lines { std::min<int>(m_editor->m_top_row + Terminal::get_instance().get_height() - BELOW_HEIGHT, m_editor->m_buffer->line_count()) };

	for (int i = m_editor->m_top_row; i < max_lines; i++) {
		std::string text = std::string(m_editor->m_buffer->line(i));
		int screen_row { i - static_cast<int>(m_editor->m_top_row) };

		Terminal::get_instance().draw_text(screen_row, offset + static_cast<int>(INDENT), text);
	}
}

void Renderer::draw_bar_below() {
	auto [width, height] = Terminal::get_instance().get_terminal_dimensions();
	auto bar_row { height - BELOW_HEIGHT };

	const std::string mode_str { std::format("-- {} --", m_editor->m_mode->get_name()) };
	const size_t spacing { 2 };

	auto cursor_str { m_editor->get_cursor().to_string() };
	auto buffer_name_str { m_editor->m_buffer->get_name() };

	auto current_count_str { std::to_string(m_editor->m_input_handler.get_count()) };

	Terminal::get_instance().draw_text(bar_row, 0, mode_str);
	Terminal::get_instance().draw_text(bar_row, mode_str.length() + spacing, buffer_name_str);
	Terminal::get_instance().draw_text(bar_row, width - cursor_str.length() - spacing, cursor_str);
	if (current_count_str != "0")
		Terminal::get_instance().draw_text(bar_row + 1, width - spacing - current_count_str.length(), current_count_str);
}

void Renderer::draw_cmd_line() {
	auto [_, height] = Terminal::get_instance().get_terminal_dimensions();
	auto is_active { m_editor->m_cmd_line.is_active };
	auto cmd_line_str { is_active ? CommandLine::COMMAND_LINE_KEY + m_editor->m_cmd_line.command : m_editor->m_cmd_line.inactive_output };

	Terminal::get_instance().draw_text(height - 1, 0, cmd_line_str);
}

int Renderer::make_relative(int line) {
	if (m_editor->m_cursor.row == line)
		return line + 1;

	return std::abs(m_editor->m_cursor.row - line);
}

std::string Renderer::format_line_number(int line, int width) {
	std::ostringstream oss;
	oss << std::setw(width) << line;
	return oss.str();
}
}
