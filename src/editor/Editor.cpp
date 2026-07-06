#include <curses.h>
#include <unistd.h>

#include <algorithm>
#include <filesystem>
#include <memory>
#include <string>

#include "Editor.hpp"

#include "CommandExecutor.hpp"
#include "RenderContextFactory.hpp"
#include "Renderer.hpp"
#include "Terminal.hpp"

#include "buffer/FileBufferSource.hpp"
#include "buffer/ISaveableBuffer.hpp"
#include "buffer/ISelectableBuffer.hpp"
#include "buffer/MemoryBufferSource.hpp"
#include "buffer/TextBuffer.hpp"

#include "modes/NormalMode.hpp"

namespace Tedit {

Editor::Editor() {
	m_buffer_manager.open_buffer(std::make_unique<TextBuffer>(std::make_unique<MemoryBufferSource>()));
	activate_current_buffer();
	change_mode(std::make_unique<NormalMode>());
}

void Editor::handle_key(int key) {
	auto action { map_key(key) };
	if (is_recording() && (!action || action->records_macro_key()))
		m_macro_controller.record_key(key);

	if (action)
		action->execute(*this);
}

std::unique_ptr<IAction> Editor::map_key(int key) {
	return get_mode()->map_action(*this, key);
}

void Editor::backspace() {
	if (m_prompt_line.is_active()) {
		m_prompt_line.backspace();
		return;
	}

	m_editing_controller.backspace(*get_buffer());
	ensure_cursor_visible();
}

void Editor::delete_char() {
	m_editing_controller.delete_char(*get_buffer());
	ensure_cursor_visible();
}

void Editor::delete_range(Cursor start, Cursor end, bool inclusive, bool linewise) {
	m_editing_controller.delete_range(*get_buffer(), start, end, inclusive, linewise);
	ensure_cursor_visible();
}

void Editor::newline() {
	if (m_prompt_line.is_active())
		return;

	m_editing_controller.newline(*get_buffer());
	ensure_cursor_visible();
}

void Editor::insert_char(char c) {
	if (m_prompt_line.is_active()) {
		m_prompt_line.insert_char(c);
		return;
	}

	m_editing_controller.insert_char(*get_buffer(), c);
	ensure_cursor_visible();
}

void Editor::move_left() {
	if (m_prompt_line.is_active()) {
		m_prompt_line.move_left();
		return;
	}

	m_cursor->col = std::max(m_cursor->col - 1, 0);
}

void Editor::move_right() {
	if (m_prompt_line.is_active()) {
		m_prompt_line.move_right();
		return;
	}

	int last_valid_column { static_cast<int>(current_line().size()) };
	m_cursor->col = std::min(m_cursor->col + 1, last_valid_column);
}

void Editor::move_up() {
	m_cursor->row = std::max(m_cursor->row - 1, 0);
	m_cursor->col = std::min(m_cursor->col, static_cast<int>(current_line().size()));

	ensure_cursor_visible();
}

void Editor::move_down() {
	int last_valid_index { static_cast<int>(get_buffer()->line_count() - 1) };
	m_cursor->row = std::min(m_cursor->row + 1, last_valid_index);
	m_cursor->col = std::min(m_cursor->col, static_cast<int>(current_line().size()));

	ensure_cursor_visible();
}

void Editor::ensure_cursor_visible() {
	if (m_cursor->row < static_cast<int>(m_top_row)) {
		m_top_row = static_cast<size_t>(m_cursor->row);
		return;
	}

	int visible_rows { std::max<int>(Terminal::get_instance().get_height() - Renderer::BELOW_HEIGHT - Renderer::ABOVE_HEIGHT, 1) };
	if (m_cursor->row >= static_cast<int>(m_top_row) + visible_rows) {
		m_top_row = static_cast<size_t>(m_cursor->row - visible_rows + 1);
	}
}

void Editor::set_cursor(const Cursor& cursor) {
	get_buffer()->set_cursor(cursor);
	ensure_cursor_visible();
}

RenderContext Editor::create_render_context() {
	return RenderContextFactory {}.create(m_buffer_manager, m_prompt_line, m_syntax_service, *m_mode, m_render_style, m_top_row);
}

std::string Editor::current_line() const {
	return std::string(get_buffer()->line(m_cursor->row));
}

void Editor::move_end_line() {
	m_cursor->col = current_line().size();
}

void Editor::change_mode(std::unique_ptr<IMode> mode) {
	Terminal::get_instance().set_cursor_shape(mode->get_cursor_shape());
	m_mode = std::move(mode);
}

bool Editor::should_close() const {
	return m_should_close;
}

void Editor::close() {
	m_buffer_manager.close();

	if (m_buffer_manager.empty()) {
		m_cursor = nullptr;
		m_top_row = 0;
		m_should_close = true;
		return;
	}

	activate_current_buffer();
}

void Editor::save_buffer() {
	save_buffer(m_buffer_manager.get_active_buffer_idx());
}

void Editor::save_buffer(size_t i) {
	auto save_buffer { get_buffer_type<ISaveableBuffer>(i) };

	if (!save_buffer) {
		m_prompt_line.set_inactive_output(std::format("buffer is not saveable {}", get_buffer(i)->get_name()));
		return;
	}

	save_buffer->save();
	m_prompt_line.set_inactive_output("saved buffer \"" + get_buffer(i)->get_name() + "\"");
}

void Editor::activate_prompt_line(std::unique_ptr<IPrompt> prompt) {
	m_prompt_line.activate(std::move(prompt));
}

void Editor::deactivate_prompt_line() {
	m_prompt_line.deactivate();
}

void Editor::submit_prompt_line() {
	m_prompt_line.submit(*this);
}

void Editor::execute_command_prompt(std::string_view input) {
	CommandExecutor::execute_command_prompt(*this, input);
}

void Editor::move_start_line() { m_cursor->col = 0; }

IMode* Editor::get_mode() { return m_mode.get(); }

void Editor::open_path(const fs::path& path, bool replace) {
	open_buffer(std::make_unique<TextBuffer>(std::make_unique<FileBufferSource>(path)), replace);
}

void Editor::open_buffer(std::unique_ptr<IBuffer> buffer, bool replace) {
	m_buffer_manager.open_buffer(std::move(buffer), replace);
	activate_current_buffer();
}

void Editor::select() {
	auto buffer { get_buffer_type<ISelectableBuffer>() };

	if (buffer)
		buffer->select(*this);
}

IBuffer* Editor::get_buffer() { return m_buffer_manager.get_buffer(); }
const IBuffer* Editor::get_buffer() const { return m_buffer_manager.get_buffer(); }

IBuffer* Editor::get_buffer(size_t i) { return m_buffer_manager.get_buffer(i); }
const IBuffer* Editor::get_buffer(size_t i) const { return m_buffer_manager.get_buffer(i); }

void Editor::switch_tab(bool next) {
	if (m_buffer_manager.empty())
		return;

	m_buffer_manager.switch_tab(next);
	activate_current_buffer();
}

void Editor::activate_current_buffer() {
	m_cursor = &get_buffer()->get_cursor();
	m_top_row = 0;
	m_last_key = 0;
}

void Editor::undo() {
	m_editing_controller.undo(*get_buffer());
	ensure_cursor_visible();
}

SyntaxService& Editor::get_syntax_service() { return m_syntax_service; }

void Editor::execute_search_prompt(std::string_view search) {
	get_buffer()->search_controller.submit(get_buffer()->lines(), search);
}

void Editor::move_to_next_search() { get_buffer()->search_controller.move_to_next(*m_cursor); }

size_t Editor::buffer_count() const {
	return m_buffer_manager.size();
}

void Editor::set_status_message(std::string_view msg) {
	m_prompt_line.set_inactive_output(msg);
}

void Editor::set_render_style(RenderStyle style) {
	m_render_style = style;
}

void Editor::stop_recording_macro() { m_macro_controller.stop_recording(); }

void Editor::start_recording_macro(char key) { m_macro_controller.start_recording(key); }

void Editor::execute_macro(char key) {
	auto keys { m_macro_controller.get_register(key) };

	if (!keys)
		return;

	for (const auto& k : *keys) {
		auto action { map_key(k) };
		if (action)
			action->execute(*this);
	}
}

bool Editor::is_recording() const { return m_macro_controller.is_recording(); }

}
