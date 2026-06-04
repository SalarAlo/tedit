#pragma once

#include <concepts>
#include <filesystem>
#include <memory>
#include <string>

#include "Cursor.hpp"
#include "HistoryAction.hpp"
#include "IBuffer.hpp"
#include "SyntaxService.hpp"

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
	void delete_range(Cursor start, Cursor end, bool inclusive = false, bool linewise = false);
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
	void save_buffer();
	void save_buffer(size_t i);

	void open_path(const fs::path& path, bool replace = true);
	void open_buffer(std::unique_ptr<IBuffer> buffer, bool replace = true);

	IBuffer* get_buffer();
	const IBuffer* get_buffer() const;

	IBuffer* get_buffer(size_t i);
	const IBuffer* get_buffer(size_t i) const;

	SyntaxService& get_syntax_service() { return m_syntax_service; }

	Mode* get_mode();

	void activate_command_line();
	void deactivate_command_line();

	void switch_tab(bool next);

	std::string current_line() const;
	void set_cursor(const Cursor& cursor);

	void undo();

private:
	void ensure_cursor_visible();
	void activate_current_buffer();
	void try_push_undo(const HistoryAction& action);

	template <buffer_type T>
	T* get_buffer_type() {
		return dynamic_cast<T*>(get_buffer());
	}

	template <buffer_type T>
	T* get_buffer_type(size_t i) {
		return dynamic_cast<T*>(m_buffers[i].get());
	}

private:
	std::vector<std::unique_ptr<IBuffer>> m_buffers {};
	size_t m_buffer_idx {};
	Cursor* m_cursor {};
	CommandLineController m_cmd_line {};
	SyntaxService m_syntax_service {};
	std::unique_ptr<Mode> m_mode {};

	int m_last_key {};
	bool m_should_close {};
	size_t m_top_row { 0 };
};

}
