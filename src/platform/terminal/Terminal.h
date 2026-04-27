#pragma once

#include "platform/terminal/CursorVisibiliy.h"

namespace Tedit {

class Terminal {
public:
	Terminal();
	~Terminal();

	void clear();
	void present();

	void draw_text(int row, int col, std::string_view text);

	void set_cursor_visibility(CursorVisibility visibility);
	void move_cursor(int row, int col);

	int read_key();
};

}
