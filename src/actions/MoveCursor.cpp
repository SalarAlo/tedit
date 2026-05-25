#include "MoveCursor.hpp"

namespace Tedit {

MoveCursor::MoveCursor(int col, int row)
    : m_row(row)
    , m_col(col) {
}

void MoveCursor::execute(Editor& e) {
	e.get_active_buffer()->get_cursor().row = m_row;
	e.get_active_buffer()->get_cursor().col = m_col;
}

}
