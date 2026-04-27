#pragma once

#include <cstddef>

#include "Terminal.h"

namespace Tedit {

class Cursor {
public:
	void left();
	void right();
	void up();
	void down();

	void apply_cursor(Terminal& terminal);

private:
	size_t m_row;
	size_t m_col;
};

}
