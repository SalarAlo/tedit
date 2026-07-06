#pragma once

#include <filesystem>
#include <memory>
#include <string>
#include <string_view>

#include "BufferManager.h"
#include "Cursor.hpp"
#include "EditingController.hpp"
#include "IBuffer.hpp"
#include "IPrompt.hpp"
#include "MacroController.hpp"
#include "RenderContext.hpp"
#include "RenderStyle.hpp"
#include "SyntaxService.hpp"

#include "modes/IMode.hpp"

#include "prompt_line/PromptLineController.hpp"

namespace fs = std::filesystem;

namespace Tedit {

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

	size_t buffer_count() const;

	SyntaxService& get_syntax_service();

	IMode* get_mode();

	void activate_prompt_line(std::unique_ptr<IPrompt> prompt);
	void deactivate_prompt_line();

	void start_recording_macro(char key);
	void stop_recording_macro();
	bool is_recording() const;
	void execute_macro(char key);

	void set_status_message(std::string_view msg);
	void set_render_style(RenderStyle style);

	void switch_tab(bool next);

	std::string current_line() const;
	void set_cursor(const Cursor& cursor);

	void undo();

	RenderContext create_render_context();

private:
	std::unique_ptr<IAction> map_key(int key);
	void ensure_cursor_visible();
	void activate_current_buffer();

	template <BufferType T>
	T* get_buffer_type() {
		return m_buffer_manager.get_buffer_type<T>();
	}

	template <BufferType T>
	T* get_buffer_type(size_t i) {
		return m_buffer_manager.get_buffer_type<T>(i);
	}

private:
	MacroController m_macro_controller {};
	BufferManager m_buffer_manager {};
	EditingController m_editing_controller {};
	Cursor* m_cursor {};

	PromptLineController m_prompt_line {};
	SyntaxService m_syntax_service {};

	std::unique_ptr<IMode> m_mode {};

	int m_last_key {};
	bool m_should_close {};
	int m_top_row { 0 };
	RenderStyle m_render_style { RenderStyle::Default };
};

}
