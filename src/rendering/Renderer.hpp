#pragma once

#include <string_view>
#include <utility>
#include <vector>

#include "DrawCall.hpp"
#include "HighlightSpan.hpp"
#include "KodaColorTheme.hpp"
#include "RenderContext.hpp"

namespace Tedit {

class Renderer {
public:
	constexpr static int INDENT { 1 };
	constexpr static int ABOVE_HEIGHT { 1 };
	constexpr static int BELOW_HEIGHT { 2 };
	constexpr static int BELOW_BAR_SPACING_RIGHT { 2 };

public:
	void render(const RenderContext& render_context);

private:
	void begin_frame(const RenderContext& render_context);
	void render_top_padding();
	void render_editor(int gutter_width);
	void position_cursor();
	void position_prompt_cursor();
	void position_editor_cursor(int gutter_width);
	void end_frame();

	void draw_gutter(int gutter_width, bool relative);
	void draw_gutter_line(const RenderLine& render_line, int gutter_width, bool relative);
	void clear_gutter_area(int screen_row, int gutter_width);
	void draw_line_number(int screen_row, int line_number, int gutter_width, bool is_current_line);

	void draw_text(int gutter_width);
	void render_visible_lines(int gutter_width);
	void render_single_line(const RenderLine& render_line, int gutter_width, const HighlightsLineMap& highlights_map);
	void clear_unused_editor_rows();
	void draw_highlighted_line(int screen_row, int col_offset, std::string_view text, std::vector<HighlightSpan> spans);

	int visual_column(std::string_view text, int byte_col) const;
	std::string expand_tabs(std::string_view text, int initial_visual_col = 0) const;

	void draw_bar_above();
	void draw_buffer_tab(int bar_row, int& offset_column, const BufferTabView& tab);
	void draw_buffer_tab_separator(int bar_row, int& offset_column);
	void draw_bar_below();
	void draw_mode_status(int bar_row, std::string_view mode);
	void draw_buffer_status(int bar_row, std::string_view mode, std::string_view buffer_name);
	DrawCall draw_cursor_status(int bar_row, int terminal_width, std::string_view cursor);
	void draw_mode_details(int bar_row, const DrawCall& cursor_position);
	void draw_prompt_line();

	HighlightsLineMap get_highlights_map() const;
	std::vector<HighlightSpan> highlights_for_line(const HighlightsLineMap& highlights_map, int line) const;

	int gutter_width() const;
	int gutter_cell_width(int gutter_width) const;
	int text_start_column(int gutter_width) const;
	int screen_row_from_buffer_row(int buffer_row) const;
	int editor_row_count() const;
	int first_unused_editor_row() const;
	int bottom_editor_row() const;
	int prompt_line_row() const;
	std::pair<int, int> cursor_screen_position(int gutter_width) const;
	std::string_view cursor_line_text() const;
	bool is_current_line(int line) const;
	int make_relative(int line);
	std::string format_line_number(int line, int width);

private:
	RenderContext m_rctx {};
	KodaColorTheme m_color_theme {};
};

}
