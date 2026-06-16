#pragma once

#include "Editor.hpp"
#include "HighlightSpan.hpp"
#include "KodaColorTheme.hpp"

namespace Tedit {

class Renderer {
public:
	constexpr static int INDENT { 1 };
	constexpr static int ABOVE_HEIGHT { 1 };
	constexpr static int BELOW_HEIGHT { 2 };
	constexpr static int BELOW_BAR_SPACING_RIGHT { 2 };

public:
	void render(Editor& editor);

private:
	void draw_gutter(int gutter_width, bool relative);
	void draw_text(int gutter_width);
	void draw_highlighted_line(int screen_row, int col_offset, std::string_view text, std::vector<HighlightSpan> spans);
	int visual_column(std::string_view text, int byte_col);
	std::string expand_tabs(std::string_view text, int initial_visual_col = 0);

	void draw_bar_above();
	void draw_bar_below();
	void draw_prompt_line();

	HighlightsLineMap get_highlights_map() const;

	int make_relative(int line);
	std::string format_line_number(int line, int width);

private:
	Editor* m_editor;
	KodaColorTheme m_color_theme {};
};

}
