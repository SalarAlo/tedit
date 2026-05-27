#include <algorithm>

#include "Renderer.hpp"

#include "DrawCall.hpp"
#include "Terminal.hpp"
#include "TextStyle.hpp"

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
		int screen_row { m_editor->m_cursor->row - static_cast<int>(m_editor->m_top_row) };
		Terminal::get_instance().move_cursor(screen_row, m_editor->m_cursor->col + static_cast<int>(offset));
	}

	Terminal::get_instance().present();
}

void Renderer::draw_gutter(size_t gutter_width, bool relative) {
	int max_lines { std::min<int>(m_editor->m_top_row + Terminal::get_instance().get_height() - BELOW_HEIGHT, m_editor->m_buffer->line_count()) };

	for (int i = m_editor->m_top_row; i < max_lines; i++) {
		bool is_current_line { i == m_editor->m_cursor->row };
		int line { relative ? make_relative(i) : i + 1 };
		int screen_row { i - static_cast<int>(m_editor->m_top_row) };

		std::string number = format_line_number(
		    line,
		    gutter_width + (is_current_line ? 0 : INDENT));

		auto row { m_editor->get_active_buffer()->get_cursor().row };

		if (i == row)
			Terminal::get_instance().enable_style(TextStyle::Bold);

		DrawCall draw_number_gutter { screen_row, 0, number };
		Terminal::get_instance().draw_text(draw_number_gutter);

		if (i == row)
			Terminal::get_instance().disable_style(TextStyle::Bold);
	}
}

void Renderer::draw_text(size_t gutter_width) {
	int offset = gutter_width + 1;
	int max_lines { std::min<int>(m_editor->m_top_row + Terminal::get_instance().get_height() - BELOW_HEIGHT, m_editor->m_buffer->line_count()) };

	for (int i = m_editor->m_top_row; i < max_lines; i++) {
		std::string text = std::string(m_editor->m_buffer->line(i));
		int screen_row { i - static_cast<int>(m_editor->m_top_row) };

		DrawCall draw_line { screen_row, offset + static_cast<int>(INDENT), text };
		Terminal::get_instance().draw_text(draw_line);
	}
}

void Renderer::draw_bar_below() {
	auto [width, height] = Terminal::get_instance().get_terminal_dimensions();
	int bar_row { static_cast<int>(height - BELOW_HEIGHT) };

	const std::string mode_str { std::format("-- {} --", m_editor->m_mode->get_name()) };

	auto cursor_str { m_editor->get_active_buffer()->get_cursor().to_string() };
	auto buffer_name_str { m_editor->m_buffer->get_name() };

	DrawCall draw_mode { bar_row, 0, mode_str };
	DrawCall draw_buffer_name { bar_row, static_cast<int>(mode_str.length() + BELOW_BAR_SPACING_RIGHT), buffer_name_str };
	DrawCall draw_cursor_position { bar_row, static_cast<int>(width - cursor_str.length() - BELOW_BAR_SPACING_RIGHT), cursor_str };

	Terminal::get_instance().draw_text(draw_mode);
	Terminal::get_instance().draw_text(draw_buffer_name);
	Terminal::get_instance().draw_text(draw_cursor_position);

	auto details { m_editor->get_mode()->get_mode_details() };

	if (details.empty())
		return;

	constexpr auto mode_details_spacing { 2 };

	DrawCall mode_details_draw_call {
		bar_row,
		static_cast<int>(draw_cursor_position.col - details.size() - mode_details_spacing),
		details
	};

	Terminal::get_instance().draw_text(mode_details_draw_call);
}

void Renderer::draw_cmd_line() {
	auto [_, height] = Terminal::get_instance().get_terminal_dimensions();
	auto is_active { m_editor->m_cmd_line.is_active };
	auto cmd_line_str { is_active ? CommandLine::COMMAND_LINE_KEY + m_editor->m_cmd_line.command : m_editor->m_cmd_line.inactive_output };

	Terminal::get_instance().draw_text(DrawCall { height - 1, 0, cmd_line_str });
}

int Renderer::make_relative(int line) {
	if (m_editor->m_cursor->row == line)
		return line + 1;

	return std::abs(m_editor->m_cursor->row - line);
}

std::string Renderer::format_line_number(int line, int width) {
	std::ostringstream oss;
	oss << std::setw(width) << line;
	return oss.str();
}
}
