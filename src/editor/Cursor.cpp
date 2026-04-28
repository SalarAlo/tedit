#include "Cursor.h"

namespace Tedit {

void Cursor::apply_cursor(Terminal& terminal) const {
	terminal.move_cursor(row, col + 2);
}

bool Cursor::is_at_beginning() const {
	return col == 0 && row == 0;
}

}
