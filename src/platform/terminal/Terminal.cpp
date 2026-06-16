#include <csignal>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <curses.h>
#include <unistd.h>

#include <array>
#include <atomic>
#include <exception>

#include "Terminal.hpp"

#include "platform/terminal/CursorShape.hpp"

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

		static constexpr char reset_cursor[] = "\033[2 q";
		[[maybe_unused]] const auto bytes_written = write(STDOUT_FILENO, reset_cursor, sizeof(reset_cursor) - 1);

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

	void write_escape(const char* sequence) {
		[[maybe_unused]] const auto bytes_written = write(STDOUT_FILENO, sequence, std::strlen(sequence));
	}

	void write_formatted_escape(const char* format, int first, int second = 0, int third = 0) {
		std::array<char, 64> buffer {};
		int length { std::snprintf(buffer.data(), buffer.size(), format, first, second, third) };
		if (length <= 0)
			return;

		[[maybe_unused]] const auto bytes_written = write(STDOUT_FILENO, buffer.data(), static_cast<size_t>(length));
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
	start_color();

	g_terminal_active.store(true);
	::refresh();
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
	write_escape("\033[2J\033[H");
}

void Terminal::clear_line(int row) {
	move_cursor(row, 0);
	write_escape("\033[2K");
}

void Terminal::clear_to_end_of_line() {
	write_escape("\033[K");
}

void Terminal::present() {
}

void Terminal::draw_text(const DrawCall& call) {
	if (call.color) {
		auto [r, g, b] = *call.color;
		Terminal::get_instance().set_fg(r, g, b);
	}

	move_cursor(call.row, call.col);
	write(STDOUT_FILENO, call.text.data(), call.text.size());

	if (call.color)
		reset_style();
}

void Terminal::set_cursor_shape(CursorShape shape) {
	switch (shape) {
	case CursorShape::Block:
		write_escape("\033[?25h");
		write_escape("\033[2 q");
		break;

	case CursorShape::Beam:
		write_escape("\033[?25h");
		write_escape("\033[6 q");
		break;

	case CursorShape::Underline:
		write_escape("\033[?25h");
		write_escape("\033[4 q");
		break;

	case CursorShape::Hidden:
		write_escape("\033[?25l");
		return;
	}
}

void Terminal::move_cursor(int row, int col) {
	write_formatted_escape("\033[%d;%dH", row + 1, col + 1);
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

void Terminal::enable_style(TextStyle style) {
	switch (style) {
	case TextStyle::Reverse:
		write_escape("\033[7m");
		break;

	case TextStyle::Bold:
		write_escape("\033[1m");
		break;

	case TextStyle::Underline:
		write_escape("\033[4m");
		break;
	}
}

void Terminal::disable_style(TextStyle style) {
	switch (style) {
	case TextStyle::Reverse:
		write_escape("\033[27m");
		break;

	case TextStyle::Bold:
		write_escape("\033[22m");
		break;

	case TextStyle::Underline:
		write_escape("\033[24m");
		break;
	}
}

void Terminal::set_fg(uint8_t r, uint8_t g, uint8_t b) {
	write_formatted_escape("\033[38;2;%d;%d;%dm", r, g, b);
}

void Terminal::reset_style() {
	write_escape("\033[0m");
}

}
