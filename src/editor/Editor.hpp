#pragma once

#include <memory>

#include "CommandLineParser.hpp"
#include "Cursor.hpp"

#include "buffer/FileBufferSource.hpp"
#include "buffer/TextBuffer.hpp"

#include "command_line/CommandLine.hpp"

#include "input/InputHandler.hpp"

#include "modes/Mode.hpp"

namespace Tedit {

class Editor {
	friend class Renderer;

public:
	Editor();

	void handle_key(int key);

	bool should_close() const;
	void close();

	void backspace();
	void delete_char();
	void newline();
	void insert_char(char c);

	void parse_and_leave_cmd_line();

	void move_left();
	void move_right();
	void move_up();
	void move_down();
	void move_end_line();
	void move_start_line();

	void change_mode(std::unique_ptr<Mode> mode);
	void save_to_buffer();

	Cursor& get_cursor();
	Mode* get_mode();

	void activate_command_line();
	void deactivate_command_line();

private:
	std::string current_line() const;

private:
	TextBuffer m_buffer { std::make_unique<FileBufferSource>("./assets/file.txt") };
	Cursor m_cursor {};
	CommandLine m_cmd_line {};
	std::unique_ptr<Mode> m_mode {};
	InputHandler m_input_handler { *this };

	int m_last_key {};
	bool m_should_close { false };
	size_t m_top_row { 0 };
};

}
