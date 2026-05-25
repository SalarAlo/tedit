#pragma once

#include "TextStyle.hpp"

#include "platform/terminal/CursorShape.hpp"

namespace Tedit {

class Terminal {
public:
	static Terminal& get_instance();
	static void emergency_restore();
	~Terminal();

	Terminal(const Terminal&) = delete;
	Terminal& operator=(const Terminal&) = delete;

	void clear();
	void present();

	void draw_text(int row, int col, std::string_view text);

	void set_cursor_shape(CursorShape shape);
	void move_cursor(int row, int col);

	std::pair<int, int> get_terminal_dimensions();
	int get_width();
	int get_height();

	int read_key();
	void enable_style(TextStyle style);

	void disable_style(TextStyle style);

private:
	Terminal();
};

}
