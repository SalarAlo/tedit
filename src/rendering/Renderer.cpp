#include "Renderer.h"

namespace Tedit {

Renderer::Renderer(Terminal& terminal)
    : m_terminal(terminal) { }

void Renderer::render(Editor& editor) {
	m_editor = &editor;

	m_terminal.clear();

	size_t gutter_width = std::to_string(m_editor->m_buffer.line_count()).size();

	draw_gutter(gutter_width, true);
	draw_text(gutter_width);
	draw_mode();

	int offset = gutter_width + m_indent + 1;
	m_terminal.move_cursor(m_editor->m_cursor.row, m_editor->m_cursor.col + offset);

	m_terminal.present();
}

void Renderer::draw_gutter(size_t gutter_width, bool relative) {
	for (int i = 0; i < m_editor->m_buffer.line_count(); i++) {
		bool is_current_line { i == m_editor->m_cursor.row };
		int line { relative ? make_relative(i) : i + 1 };

		std::string number = format_line_number(
		    line,
		    gutter_width + (is_current_line ? 0 : m_indent));

		m_terminal.draw_text(i, 0, number);
	}
}

void Renderer::draw_text(size_t gutter_width) {
	int offset = gutter_width + 1;

	for (int i = 0; i < m_editor->m_buffer.line_count(); i++) {
		std::string text = std::string(m_editor->m_buffer.line(i));

		m_terminal.draw_text(i, offset + m_indent, text);
	}
}

void Renderer::draw_mode() {
	auto [_, height] = m_terminal.get_terminal_dimensions();
	auto mode_str { "-- " + m_editor->m_mode->get_name() + " --" };
	m_terminal.draw_text(height - 1, 0, mode_str);
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
