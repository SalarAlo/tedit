#include "Cursor.hpp"

namespace Tedit {

void Cursor::apply_cursor(Terminal& terminal) const {
	terminal.move_cursor(row, col + 2);
}

bool Cursor::is_at_beginning() const {
	return col == 0 && row == 0;
}

std::string Cursor::to_string() const { return std::to_string(row + 1) + ":" + std::to_string(col); }

}
