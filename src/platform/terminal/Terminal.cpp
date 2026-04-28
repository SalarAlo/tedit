#include "Terminal.h"

#include "platform/terminal/CursorVisibiliy.h"

namespace Tedit {

Terminal::Terminal() {
	if (initscr() == nullptr) {
		throw std::runtime_error("Failed to initialise Terminal UI.");
	}

	cbreak();
	noecho();
	keypad(stdscr, true);

	set_cursor_visibility(CursorVisibility::Visible);
}

Terminal::~Terminal() {
	endwin();
}

void Terminal::clear() {
	::clear();
}

void Terminal::present() {
	refresh();
}

void Terminal::draw_text(int row, int col, std::string_view text) {
	mvaddnstr(row, col, text.data(), static_cast<int>(text.size()));
}

void Terminal::set_cursor_visibility(CursorVisibility visibility) {
	curs_set(static_cast<int>(visibility));
}

void Terminal::move_cursor(int row, int col) {
	move(row, col);
}

int Terminal::read_key() {
	return getch();
}

}
