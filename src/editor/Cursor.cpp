#include "Cursor.h"

namespace Tedit {

void Cursor::left() {
	m_col--;
}

void Cursor::right() {
	m_col++;
}

void Cursor::up() {
	m_row++;
}

void Cursor::down() {
	m_row--;
}

void Cursor::apply_cursor(Terminal& terminal) {
	terminal.move_cursor(m_row, m_col);
}

}
