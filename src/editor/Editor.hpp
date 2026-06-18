#pragma once

#include <concepts>
#include <filesystem>
#include <memory>
#include <string>

#include "Cursor.hpp"
#include "HistoryAction.hpp"
#include "IBuffer.hpp"
#include "IPrompt.hpp"
#include "RenderContext.hpp"
#include "SyntaxService.hpp"

#include "modes/IMode.hpp"

#include "prompt_line/PromptLineController.hpp"

namespace fs = std::filesystem;

namespace Tedit {

template <typename T>
concept BufferType = std::derived_from<T, IBuffer>;

class Editor {
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

	void submit_prompt_line();
	void execute_command_prompt(std::string_view input);
	void execute_search_prompt(std::string_view search);
	void move_to_next_search();
	void select();

	void move_left();
	void move_right();
	void move_up();
	void move_down();
	void move_end_line();
	void move_start_line();

	void change_mode(std::unique_ptr<IMode> mode);
	void save_buffer();
	void save_buffer(size_t i);

	void open_path(const fs::path& path, bool replace = true);
	void open_buffer(std::unique_ptr<IBuffer> buffer, bool replace = true);

	IBuffer* get_buffer();
	const IBuffer* get_buffer() const;

	IBuffer* get_buffer(size_t i);
	const IBuffer* get_buffer(size_t i) const;

	SyntaxService& get_syntax_service();

	IMode* get_mode();

	void activate_prompt_line(std::unique_ptr<IPrompt> prompt);
	void deactivate_prompt_line();

	void switch_tab(bool next);

	std::string current_line() const;
	void set_cursor(const Cursor& cursor);

	void undo();

	RenderContext create_render_context();

private:
	void ensure_cursor_visible();
	void activate_current_buffer();
	void try_push_undo(const HistoryAction& action);

	template <BufferType T>
	T* get_buffer_type() {
		return dynamic_cast<T*>(get_buffer());
	}

	template <BufferType T>
	T* get_buffer_type(size_t i) {
		return dynamic_cast<T*>(m_buffers[i].get());
	}

private:
	std::vector<std::unique_ptr<IBuffer>> m_buffers {};

	size_t m_buffer_idx {};
	Cursor* m_cursor {};

	PromptLineController m_prompt_line {};
	SyntaxService m_syntax_service {};

	std::unique_ptr<IMode> m_mode {};

	int m_last_key {};
	bool m_should_close {};
	int m_top_row { 0 };
};

}
