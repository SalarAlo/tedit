#pragma once

#include "Editor.hpp"

namespace Tedit {

class Renderer {
public:
	constexpr static size_t INDENT { 1 };
	constexpr static size_t BELOW_HEIGHT { 2 };
	constexpr static int BELOW_BAR_SPACING_RIGHT { 2 };

public:
	void render(Editor& editor);

private:
	void draw_gutter(size_t gutter_width, bool relative);
	void draw_text(size_t gutter_width);
	void draw_bar_below();
	void draw_cmd_line();

	int make_relative(int line);
	std::string format_line_number(int line, int width);

private:
	Editor* m_editor;
};

}
