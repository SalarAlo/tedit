#include <curses.h>

#include <algorithm>
#include <vector>

#include "Renderer.hpp"

#include "ColorRGB.hpp"
#include "DrawCall.hpp"
#include "HighlightKind.hpp"
#include "HighlightSpan.hpp"
#include "RenderContext.hpp"
#include "Terminal.hpp"
#include "TextStyle.hpp"

namespace Tedit {

void Renderer::render(const RenderContext& render_context) {
	begin_frame(render_context);

	int line_number_width { gutter_width() };

	draw_bar_above();
	render_editor(line_number_width);
	draw_bar_below();
	draw_prompt_line();
	position_cursor();
	end_frame();
}

void Renderer::begin_frame(const RenderContext& render_context) {
	m_rctx = render_context;

	Terminal::get_instance().set_cursor_shape(CursorShape::Hidden);
	render_top_padding();
}

void Renderer::render_top_padding() {
	for (int row {}; row < std::min<int>(ABOVE_HEIGHT, Terminal::get_instance().get_height()); ++row)
		Terminal::get_instance().clear_line(row);
}

void Renderer::render_editor(int gutter_width) {
	draw_gutter(gutter_width, true);
	draw_text(gutter_width);
}

void Renderer::position_cursor() {
	if (m_rctx.prompt.active) {
		position_prompt_cursor();
		return;
	}

	position_editor_cursor(gutter_width());
}

void Renderer::position_prompt_cursor() {
	Terminal::get_instance().move_cursor(prompt_line_row(), m_rctx.prompt.cursor_col);
}

void Renderer::position_editor_cursor(int gutter_width) {
	auto [screen_row, screen_col] = cursor_screen_position(gutter_width);
	Terminal::get_instance().move_cursor(screen_row, screen_col);
}

void Renderer::end_frame() {
	Terminal::get_instance().set_cursor_shape(m_rctx.cursor_shape);
	Terminal::get_instance().present();
}

void Renderer::draw_gutter(int gutter_width, bool relative) {
	for (const auto& render_line : m_rctx.visible_lines)
		draw_gutter_line(render_line, gutter_width, relative);
}

void Renderer::draw_gutter_line(const RenderLine& render_line, int gutter_width, bool relative) {
	int line_number { relative ? make_relative(render_line.row) : render_line.row + 1 };
	int screen_row { screen_row_from_buffer_row(render_line.row) };
	bool current_line { is_current_line(render_line.row) };

	clear_gutter_area(screen_row, gutter_width);
	draw_line_number(screen_row, line_number, gutter_width, current_line);
}

void Renderer::clear_gutter_area(int screen_row, int gutter_width) {
	Terminal::get_instance().draw_text(DrawCall {
	    screen_row,
	    0,
	    std::string(static_cast<size_t>(text_start_column(gutter_width)), ' ') });
}

void Renderer::draw_line_number(int screen_row, int line_number, int gutter_width, bool is_current_line) {
	std::string number { format_line_number(line_number, gutter_cell_width(gutter_width)) };

	if (is_current_line)
		Terminal::get_instance().enable_style(TextStyle::Bold);

	DrawCall draw_number_gutter {
		screen_row,
		0,
		number,
		is_current_line ? ColorRGB { 232, 187, 90 } : Colors::WHITE
	};
	Terminal::get_instance().draw_text(draw_number_gutter);

	if (is_current_line)
		Terminal::get_instance().disable_style(TextStyle::Bold);
}

void Renderer::draw_text(int gutter_width) {
	render_visible_lines(gutter_width);
	clear_unused_editor_rows();
}

void Renderer::render_visible_lines(int gutter_width) {
	auto highlights_map { get_highlights_map() };

	for (const auto& render_line : m_rctx.visible_lines)
		render_single_line(render_line, gutter_width, highlights_map);
}

void Renderer::render_single_line(const RenderLine& render_line, int gutter_width, const HighlightsLineMap& highlights_map) {
	draw_highlighted_line(
	    screen_row_from_buffer_row(render_line.row),
	    text_start_column(gutter_width),
	    render_line.text,
	    highlights_for_line(highlights_map, render_line.row));
}

void Renderer::clear_unused_editor_rows() {
	for (int screen_row { first_unused_editor_row() }; screen_row < bottom_editor_row(); ++screen_row)
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

int Renderer::visual_column(std::string_view text, int byte_col) const {
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

std::string Renderer::expand_tabs(std::string_view text, int initial_visual_col) const {
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
	auto buffer_amount { m_rctx.tabs.size() };
	for (size_t i {}; i < buffer_amount; i++) {
		draw_buffer_tab(bar_row, offset_column, m_rctx.tabs[i]);

		if (i != buffer_amount - 1)
			draw_buffer_tab_separator(bar_row, offset_column);
	}
}

void Renderer::draw_buffer_tab(int bar_row, int& offset_column, const BufferTabView& tab) {
	std::string display_name { std::format("[{}]", tab.name) };

	DrawCall current_buffer_draw {
		bar_row,
		offset_column,
		display_name,
		m_color_theme.color_for(tab.active ? HighlightKind::Active : HighlightKind::Unknown)
	};
	Terminal::get_instance().draw_text(current_buffer_draw);
	offset_column += display_name.size();
}

void Renderer::draw_buffer_tab_separator(int bar_row, int& offset_column) {
	std::string seperator { " | " };

	Terminal::get_instance().draw_text({ bar_row, offset_column, seperator });
	offset_column += seperator.size();
}

void Renderer::draw_bar_below() {
	auto [width, height] = Terminal::get_instance().get_terminal_dimensions();
	int bar_row { height - BELOW_HEIGHT };
	Terminal::get_instance().clear_line(bar_row);

	const std::string mode_str { std::format("-- {} --", m_rctx.mode_name) };
	auto cursor_str { m_rctx.active_cursor.to_string() };
	auto buffer_name_str { m_rctx.active_buffer_name };

	draw_mode_status(bar_row, mode_str);
	draw_buffer_status(bar_row, mode_str, buffer_name_str);
	auto cursor_position { draw_cursor_status(bar_row, width, cursor_str) };
	draw_mode_details(bar_row, cursor_position);
}

void Renderer::draw_mode_status(int bar_row, std::string_view mode) {
	Terminal::get_instance().draw_text(DrawCall { bar_row, 0, std::string(mode) });
}

void Renderer::draw_buffer_status(int bar_row, std::string_view mode, std::string_view buffer_name) {
	Terminal::get_instance().draw_text(DrawCall {
	    bar_row,
	    static_cast<int>(mode.length() + BELOW_BAR_SPACING_RIGHT),
	    std::string(buffer_name) });
}

DrawCall Renderer::draw_cursor_status(int bar_row, int terminal_width, std::string_view cursor) {
	DrawCall cursor_position {
		bar_row,
		static_cast<int>(terminal_width - cursor.length() - BELOW_BAR_SPACING_RIGHT),
		std::string(cursor)
	};
	Terminal::get_instance().draw_text(cursor_position);
	return cursor_position;
}

void Renderer::draw_mode_details(int bar_row, const DrawCall& cursor_position) {
	auto details { m_rctx.mode_details };

	if (details.empty())
		return;

	constexpr auto mode_details_spacing { 2 };

	DrawCall mode_details_draw_call {
		bar_row,
		static_cast<int>(cursor_position.col - details.size() - mode_details_spacing),
		details
	};

	Terminal::get_instance().draw_text(mode_details_draw_call);
}

void Renderer::draw_prompt_line() {
	Terminal::get_instance().clear_line(prompt_line_row());
	Terminal::get_instance().draw_text(DrawCall { prompt_line_row(), 0, m_rctx.prompt.text });
}

int Renderer::make_relative(int line) {
	if (m_rctx.active_cursor.row == line)
		return line + 1;

	return std::abs(m_rctx.active_cursor.row - line);
}

std::string Renderer::format_line_number(int line, int width) {
	std::ostringstream oss;
	oss << std::setw(width) << line;
	return oss.str();
}

HighlightsLineMap Renderer::get_highlights_map() const {
	HighlightsLineMap spans_by_row {};

	for (const auto& span : m_rctx.highlights)
		spans_by_row[span.row].push_back(span);

	return spans_by_row;
}

std::vector<HighlightSpan> Renderer::highlights_for_line(const HighlightsLineMap& highlights_map, int line) const {
	auto it = highlights_map.find(line);
	if (it == highlights_map.end())
		return {};

	return it->second;
}

int Renderer::gutter_width() const {
	auto length { std::to_string(m_rctx.active_buffer_line_count).size() };
	return std::max<int>(2, length);
}

int Renderer::gutter_cell_width(int gutter_width) const {
	return gutter_width + INDENT;
}

int Renderer::text_start_column(int gutter_width) const {
	return gutter_cell_width(gutter_width) + INDENT;
}

int Renderer::screen_row_from_buffer_row(int buffer_row) const {
	return buffer_row - m_rctx.top_row + ABOVE_HEIGHT;
}

int Renderer::editor_row_count() const {
	return std::max<int>(Terminal::get_instance().get_height() - BELOW_HEIGHT - ABOVE_HEIGHT, 1);
}

int Renderer::first_unused_editor_row() const {
	return static_cast<int>(m_rctx.visible_lines.size()) + ABOVE_HEIGHT;
}

int Renderer::bottom_editor_row() const {
	return editor_row_count() + ABOVE_HEIGHT;
}

int Renderer::prompt_line_row() const {
	auto [_, height] = Terminal::get_instance().get_terminal_dimensions();
	return height - 1;
}

std::pair<int, int> Renderer::cursor_screen_position(int gutter_width) const {
	return {
		screen_row_from_buffer_row(m_rctx.active_cursor.row),
		visual_column(cursor_line_text(), m_rctx.active_cursor.col) + text_start_column(gutter_width)
	};
}

std::string_view Renderer::cursor_line_text() const {
	for (const auto& line : m_rctx.visible_lines) {
		if (line.row == m_rctx.active_cursor.row)
			return line.text;
	}

	return {};
}

bool Renderer::is_current_line(int line) const {
	return m_rctx.active_cursor.row == line;
}

}
