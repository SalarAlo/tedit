#pragma once

#include "platform/terminal/CursorVisibiliy.h"

namespace Tedit {

class Terminal {
public:
	static Terminal& get_instance();
	~Terminal();

	Terminal(const Terminal&) = delete;
	Terminal& operator=(const Terminal&) = delete;

	void clear();
	void present();

	void draw_text(int row, int col, std::string_view text);

	void set_cursor_visibility(CursorVisibility visibility);
	void move_cursor(int row, int col);

	std::pair<int, int> get_terminal_dimensions();

	int read_key();

private:
	Terminal();
};

}
