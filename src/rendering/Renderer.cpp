#include "Renderer.h"

namespace Tedit {

void Renderer::render(Editor& editor) {
	m_editor = &editor;

	Terminal::get_instance().clear();

	size_t gutter_width = std::to_string(m_editor->m_buffer.line_count()).size();

	draw_gutter(gutter_width, true);
	draw_text(gutter_width);
	draw_mode();
	draw_cmd_line();

	if (m_editor->m_cmd_line.is_active) {
		auto [_, height] = Terminal::get_instance().get_terminal_dimensions();
		Terminal::get_instance().move_cursor(height - 1, m_editor->m_cmd_line.cursor_col);
	} else {
		int offset = gutter_width + m_indent + 1;
		Terminal::get_instance().move_cursor(m_editor->m_cursor.row, m_editor->m_cursor.col + offset);
	}

	Terminal::get_instance().present();
}

void Renderer::draw_gutter(size_t gutter_width, bool relative) {
	for (int i = 0; i < m_editor->m_buffer.line_count(); i++) {
		bool is_current_line { i == m_editor->m_cursor.row };
		int line { relative ? make_relative(i) : i + 1 };

		std::string number = format_line_number(
		    line,
		    gutter_width + (is_current_line ? 0 : m_indent));

		Terminal::get_instance().draw_text(i, 0, number);
	}
}

void Renderer::draw_text(size_t gutter_width) {
	int offset = gutter_width + 1;

	for (int i = 0; i < m_editor->m_buffer.line_count(); i++) {
		std::string text = std::string(m_editor->m_buffer.line(i));

		Terminal::get_instance().draw_text(i, offset + m_indent, text);
	}
}

void Renderer::draw_mode() {
	auto [_, height] = Terminal::get_instance().get_terminal_dimensions();
	auto mode_str { "-- " + m_editor->m_mode->get_name() + " --" };
	Terminal::get_instance().draw_text(height - 2, 0, mode_str);
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
