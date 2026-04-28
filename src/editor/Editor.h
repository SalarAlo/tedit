#pragma once

#include "Cursor.h"
#include "FileBufferSource.h"
#include "TextBuffer.h"

#include "platform/terminal/Terminal.h"

namespace Tedit {
class Editor {
public:
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

	std::string current_line() const;
	int make_relative(int line);

private:
	int m_last_key {};
	Terminal m_terminal {};
	TextBuffer m_buffer { std::make_unique<FileBufferSource>("./assets/file.txt") };
	Cursor m_cursor {};
};

}
