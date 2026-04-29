#pragma once

#include "Cursor.h"
#include "FileBufferSource.h"
#include "TextBuffer.h"

#include "modes/Mode.h"

#include "platform/terminal/Terminal.h"

namespace Tedit {

class Editor {
public:
	Editor();

	bool should_close() const;
	void poll_events();

	void update();
	void draw();

	void backspace();
	void newline();
	void insert_char(char c);

	void move_left();
	void move_right();
	void move_up();
	void move_down();
	void move_end_line();

	void change_mode(std::unique_ptr<Mode> mode);

private:
	std::string current_line() const;
	int make_relative(int line);

	void draw_gutter(size_t gutter_width, bool relative);
	void draw_text(size_t gutter_width);

	std::string format_line_number(int line, int width);

private:
	int m_last_key {};
	Terminal m_terminal {};
	TextBuffer m_buffer { std::make_unique<FileBufferSource>("./assets/file.txt") };
	Cursor m_cursor {};
	std::unique_ptr<Mode> m_mode {};

	size_t m_indent { 1 };
};

}
