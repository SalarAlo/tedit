#pragma once

#include <concepts>
#include <filesystem>
#include <memory>
#include <string>

#include "Cursor.hpp"
#include "IBuffer.hpp"
#include "MemoryBufferSource.hpp"

#include "buffer/TextBuffer.hpp"

#include "command_line/CommandLineController.hpp"

#include "modes/Mode.hpp"

namespace fs = std::filesystem;

namespace Tedit {

template <typename T>
concept buffer_type = std::derived_from<T, IBuffer>;

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

	void exec_and_leave_cmd_line();
	void select();

	void move_left();
	void move_right();
	void move_up();
	void move_down();
	void move_end_line();
	void move_start_line();

	void change_mode(std::unique_ptr<Mode> mode);
	bool try_save_to_buffer();

	void open_path(const fs::path& path);
	void open_buffer(std::unique_ptr<IBuffer> buffer);

	IBuffer* get_active_buffer();
	const IBuffer* get_active_buffer() const;
	Mode* get_mode();

	void activate_command_line();
	void deactivate_command_line();

	std::string current_line() const;
	void set_cursor(const Cursor& cursor);

private:
	void ensure_cursor_visible();

	template <buffer_type T>
	T* get_buffer_type() {
		return dynamic_cast<T*>(m_buffer.get());
	}

private:
	std::unique_ptr<IBuffer> m_buffer { std::make_unique<TextBuffer>(std::make_unique<MemoryBufferSource>()) };
	Cursor* m_cursor {};
	CommandLineController m_cmd_line {};
	std::unique_ptr<Mode> m_mode {};

	int m_last_key {};
	bool m_should_close { false };
	size_t m_top_row { 0 };
};

}
