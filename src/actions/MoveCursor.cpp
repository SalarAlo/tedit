#include "MoveCursor.h"

namespace Tedit {

MoveCursor::MoveCursor(int col, int row)
    : m_row(row)
    , m_col(col) {
}

void MoveCursor::execute(Editor& e) {
	e.get_cursor().row = m_row;
	e.get_cursor().col = m_col;
}

}
