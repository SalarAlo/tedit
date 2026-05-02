#pragma once

#include <memory>

#include "Cursor.h"

#include "buffer/FileBufferSource.h"
#include "buffer/TextBuffer.h"

#include "command_line/CommandLine.h"

#include "modes/Mode.h"

namespace Tedit {

class Editor {
	friend class Renderer;

public:
	Editor();

	void handle_key(int key);

	bool should_close() const;
	void close();

	void backspace();
	void newline();
	void insert_char(char c);

	void move_left();
	void move_right();
	void move_up();
	void move_down();
	void move_end_line();

	void change_mode(std::unique_ptr<Mode> mode);
	void save_to_buffer();

	Cursor& get_cursor();

	void activate_command_line();
	void deactivate_command_line();

private:
	std::string current_line() const;

private:
	int m_last_key {};
	TextBuffer m_buffer { std::make_unique<FileBufferSource>("./assets/file.txt") };
	Cursor m_cursor {};
	std::unique_ptr<Mode> m_mode {};

	bool m_should_close {};
	CommandLine m_cmd_line {};
};

}
