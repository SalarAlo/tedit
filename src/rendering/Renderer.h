#pragma once

#include "Editor.h"
#include "Terminal.h"

namespace Tedit {

class Renderer {
public:
	Renderer(Terminal& terminal);

	void render(Editor& editor);

private:
	void draw_gutter(size_t gutter_width, bool relative);
	void draw_text(size_t gutter_width);
	void draw_mode();

	int make_relative(int line);
	std::string format_line_number(int line, int width);

private:
	Terminal& m_terminal;
	Editor* m_editor;
	size_t m_indent { 1 };
};

}
