#include "Terminal.hpp"

#include "platform/terminal/CursorShape.hpp"

#include <atomic>
#include <csignal>
#include <cstdlib>
#include <exception>
#include <unistd.h>

namespace Tedit {

namespace {

std::atomic<bool> g_terminal_active { false };
std::atomic<bool> g_terminal_restored { false };
std::atomic<bool> g_handlers_installed { false };

void restore_terminal_state() {
	if (!g_terminal_active.load()) {
		return;
	}

	keypad(stdscr, false);
	nocbreak();
	echo();
	curs_set(1);

	// Restore a standard block cursor even if we crash mid-render.
	static constexpr char kResetCursor[] = "\033[2 q";
	[[maybe_unused]] const auto bytes_written =
		write(STDOUT_FILENO, kResetCursor, sizeof(kResetCursor) - 1);

	reset_shell_mode();
	endwin();

	g_terminal_active.store(false);
}

void emergency_cleanup_and_exit(int signal_number) {
	Terminal::emergency_restore();
	std::signal(signal_number, SIG_DFL);
	std::raise(signal_number);
}

void install_abort_handlers() {
	if (g_handlers_installed.exchange(true)) {
		return;
	}

	std::atexit([]() { Terminal::emergency_restore(); });
	std::set_terminate([]() {
		Terminal::emergency_restore();
		std::_Exit(EXIT_FAILURE);
	});

	std::signal(SIGABRT, emergency_cleanup_and_exit);
	std::signal(SIGINT, emergency_cleanup_and_exit);
	std::signal(SIGSEGV, emergency_cleanup_and_exit);
	std::signal(SIGTERM, emergency_cleanup_and_exit);
	std::signal(SIGILL, emergency_cleanup_and_exit);
	std::signal(SIGFPE, emergency_cleanup_and_exit);
}

}

Terminal::Terminal() {
	if (initscr() == nullptr) {
		throw std::runtime_error("Failed to initialise Terminal UI.");
	}

	install_abort_handlers();
	set_escdelay(0);
	cbreak();
	noecho();
	keypad(stdscr, true);
	g_terminal_active.store(true);
}

Terminal::~Terminal() {
	emergency_restore();
}

void Terminal::emergency_restore() {
	if (g_terminal_restored.exchange(true)) {
		return;
	}

	restore_terminal_state();
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

void Terminal::set_cursor_shape(CursorShape shape) {
	switch (shape) {
	case CursorShape::Block:
		printf("\033[2 q");
		break;

	case CursorShape::Beam:
		printf("\033[6 q");
		break;

	case CursorShape::Underline:
		printf("\033[4 q");
		break;

	case CursorShape::Hidden:
		curs_set(0);
		return;
	}

	fflush(stdout);
}

void Terminal::move_cursor(int row, int col) {
	move(row, col);
}

int Terminal::read_key() {
	return getch();
}

std::pair<int, int> Terminal::get_terminal_dimensions() {
	int rows, cols;
	getmaxyx(stdscr, rows, cols);
	return std::make_pair(cols, rows);
}

Terminal& Terminal::get_instance() {
	static Terminal instance {};

	return instance;
}

int Terminal::get_width() { return get_terminal_dimensions().first; }

int Terminal::get_height() { return get_terminal_dimensions().second; }

}
