#include <curses.h>

#include <algorithm>
#include <vector>

#include "Renderer.hpp"

#include "ColorRGB.hpp"
#include "DrawCall.hpp"
#include "HighlightKind.hpp"
#include "HighlightSpan.hpp"
#include "Terminal.hpp"
#include "TextBuffer.hpp"
#include "TextStyle.hpp"

namespace Tedit {

void Renderer::render(Editor& editor) {
	m_editor = &editor;
	Terminal::get_instance().set_cursor_shape(CursorShape::Hidden);
	for (int row {}; row < std::min<int>(ABOVE_HEIGHT, Terminal::get_instance().get_height()); ++row)
		Terminal::get_instance().clear_line(row);

	auto length { std::to_string(m_editor->get_buffer()->line_count()).size() };
	int gutter_width { std::max<int>(2, length) };

	draw_bar_above();
	draw_gutter(gutter_width, true);
	draw_text(gutter_width);
	draw_bar_below();
	draw_prompt_line();

	if (m_editor->m_prompt_line.is_active()) {
		auto [_, height] = Terminal::get_instance().get_terminal_dimensions();
		Terminal::get_instance().move_cursor(height - 1, m_editor->m_prompt_line.cursor_col());
	} else {
		int offset { gutter_width + INDENT + 1 };
		int screen_row { m_editor->m_cursor->row - m_editor->m_top_row + ABOVE_HEIGHT };
		Terminal::get_instance().move_cursor(screen_row, visual_column(m_editor->current_line(), m_editor->m_cursor->col) + offset);
	}

	Terminal::get_instance().set_cursor_shape(m_editor->m_mode->get_cursor_shape());
	Terminal::get_instance().present();
}

void Renderer::draw_gutter(int gutter_width, bool relative) {
	int editor_rows { std::max<int>(Terminal::get_instance().get_height() - BELOW_HEIGHT - ABOVE_HEIGHT, 1) };
	int max_lines { std::min<int>(m_editor->m_top_row + editor_rows, m_editor->get_buffer()->line_count()) };
	int gutter_cell_width { gutter_width + INDENT };
	int text_start_col { gutter_cell_width + INDENT };

	for (int i = m_editor->m_top_row; i < max_lines; i++) {
		bool is_current_line { i == m_editor->m_cursor->row };
		int line { relative ? make_relative(i) : i + 1 };
		int screen_row { i - m_editor->m_top_row + ABOVE_HEIGHT };

		Terminal::get_instance().draw_text(DrawCall { screen_row, 0, std::string(static_cast<size_t>(text_start_col), ' ') });

		std::string number = format_line_number(line, gutter_cell_width);

		auto row { m_editor->get_buffer()->get_cursor().row };

		if (i == row)
			Terminal::get_instance().enable_style(TextStyle::Bold);

		DrawCall draw_number_gutter { screen_row, 0, number, is_current_line ? ColorRGB { 232, 187, 90 } : Colors::WHITE };
		Terminal::get_instance().draw_text(draw_number_gutter);

		if (i == row)
			Terminal::get_instance().disable_style(TextStyle::Bold);
	}
}

void Renderer::draw_text(int gutter_width) {
	int offset_left { gutter_width + 1 + INDENT };
	int lines_amount { std::max<int>(Terminal::get_instance().get_height() - BELOW_HEIGHT - ABOVE_HEIGHT, 1) };
	int last_rendered_line { std::min<int>(m_editor->m_top_row + lines_amount, m_editor->get_buffer()->line_count()) };

	auto highlights_map { get_highlights_map() };

	for (int i = m_editor->m_top_row; i < last_rendered_line; ++i) {
		std::string text { std::string(m_editor->get_buffer()->line(i)) };
		int screen_row { i - m_editor->m_top_row + ABOVE_HEIGHT };

		auto it = highlights_map.find(i);

		std::vector<HighlightSpan> line_spans {};
		if (it != highlights_map.end())
			line_spans = it->second;

		draw_highlighted_line(screen_row, offset_left, text, std::move(line_spans));
	}

	auto first_empty_editor_row { last_rendered_line - m_editor->m_top_row + ABOVE_HEIGHT };
	auto bottom_of_editor_buffer { lines_amount + ABOVE_HEIGHT };
	for (int screen_row { first_empty_editor_row }; screen_row < bottom_of_editor_buffer; ++screen_row)
		Terminal::get_instance().clear_line(screen_row);
}

void Renderer::draw_highlighted_line(int screen_row, int col_offset, std::string_view text, std::vector<HighlightSpan> spans) {
	auto expanded_text { expand_tabs(text) };

	Terminal::get_instance().draw_text(DrawCall { screen_row, col_offset, expanded_text });
	Terminal::get_instance().clear_to_end_of_line();

	if (spans.empty()) {
		return;
	}

	const int line_length { static_cast<int>(text.size()) };

	for (const auto& span : spans) {
		int start_col { std::clamp(span.start_col, 0, line_length) };
		int past_end_col { std::clamp(span.past_end_col, 0, line_length) };

		if (past_end_col <= start_col)
			continue;

		int start_visual_col { visual_column(text, start_col) };
		auto highlighted_text { text.substr(start_col, past_end_col - start_col) };

		Terminal::get_instance().draw_text(DrawCall {
		    screen_row,
		    col_offset + start_visual_col,
		    expand_tabs(highlighted_text, start_visual_col),
		    m_color_theme.color_for(span.kind),
		});
	}
}

int Renderer::visual_column(std::string_view text, int byte_col) {
	constexpr int tab_width { 8 };
	int visual_col {};
	int max_col { std::min(byte_col, static_cast<int>(text.size())) };

	for (int i {}; i < max_col; ++i) {
		if (text[i] == '\t') {
			visual_col += tab_width - (visual_col % tab_width);
			continue;
		}

		visual_col++;
	}

	return visual_col;
}

std::string Renderer::expand_tabs(std::string_view text, int initial_visual_col) {
	constexpr int tab_width { 8 };
	std::string expanded {};
	int visual_col { initial_visual_col };

	for (const char c : text) {
		if (c == '\t') {
			int spaces { tab_width - (visual_col % tab_width) };
			expanded.append(static_cast<size_t>(spaces), ' ');
			visual_col += spaces;
			continue;
		}

		expanded += c;
		visual_col++;
	}

	return expanded;
}

void Renderer::draw_bar_above() {
	int bar_row { 0 };
	Terminal::get_instance().clear_line(bar_row);

	int offset_column {};
	auto buffer_amount { m_editor->m_buffers.size() };
	for (size_t i {}; i < buffer_amount; i++) {
		auto* buffer { m_editor->m_buffers[i].get() };
		bool is_active { i == m_editor->m_buffer_idx };
		std::string display_name { std::format("[{}]", buffer->get_name()) };
		std::string seperator { " | " };

		DrawCall current_buffer_draw { bar_row, offset_column, display_name, m_color_theme.color_for(is_active ? HighlightKind::Active : HighlightKind::Unknown) };
		Terminal::get_instance().draw_text(current_buffer_draw);
		offset_column += display_name.size();

		if (i != buffer_amount - 1) {
			Terminal::get_instance().draw_text({ bar_row, offset_column, seperator });
			offset_column += seperator.size();
		}
	}
}

void Renderer::draw_bar_below() {
	auto [width, height] = Terminal::get_instance().get_terminal_dimensions();
	int bar_row { height - BELOW_HEIGHT };
	Terminal::get_instance().clear_line(bar_row);

	const std::string mode_str { std::format("-- {} --", m_editor->m_mode->get_name()) };

	auto cursor_str { m_editor->get_buffer()->get_cursor().to_string() };
	auto buffer_name_str { m_editor->get_buffer()->get_name() };

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

void Renderer::draw_prompt_line() {
	auto [_, height] = Terminal::get_instance().get_terminal_dimensions();
	Terminal::get_instance().clear_line(height - 1);
	auto is_active { m_editor->m_prompt_line.is_active() };

	std::string prompt_line_str {
		is_active
		    ? std::format("{}{}", m_editor->m_prompt_line.activation_char(), m_editor->m_prompt_line.input())
		    : m_editor->m_prompt_line.inactive_output()
	};

	Terminal::get_instance().draw_text(DrawCall { height - 1, 0, prompt_line_str });
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

HighlightsLineMap Renderer::get_highlights_map() const {
	std::vector<HighlightSpan> all_spans {};

	if (auto text_buffer { dynamic_cast<const TextBuffer*>(m_editor->get_buffer()) })
		all_spans = m_editor->get_syntax_service().highlight(*text_buffer);

	HighlightsLineMap spans_by_row {};

	for (const auto& span : all_spans)
		spans_by_row[span.row].push_back(span);

	const auto& search_occurrences { m_editor->get_buffer()->search_controller.get_occurrences() };
	for (const auto& search_occurrence : search_occurrences) {
		spans_by_row[search_occurrence.row].emplace_back(
		    HighlightKind::SearchOccurrence,
		    search_occurrence.col,
		    search_occurrence.col + 1,

		    search_occurrence.row);
	}

	return spans_by_row;
}

}
