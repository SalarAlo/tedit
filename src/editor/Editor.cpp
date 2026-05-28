#include <curses.h>

#include <filesystem>
#include <memory>

#include "Editor.hpp"

#include "CommandLineParser.hpp"
#include "DirectoryBuffer.hpp"
#include "FileBufferSource.hpp"
#include "IEditBuffer.hpp"
#include "ISaveableBuffer.hpp"
#include "ISelectableBuffer.hpp"
#include "NormalMode.hpp"
#include "Renderer.hpp"
#include "Terminal.hpp"

namespace Tedit {

Editor::Editor() {
	m_cursor = &m_buffer->get_cursor();
	change_mode(std::make_unique<NormalMode>());
}

void Editor::handle_key(int key) {
	auto action { get_mode()->map_action(key) };
	if (action)
		action->execute(*this);
}

void Editor::backspace() {
	if (m_cmd_line.is_active()) {
		m_cmd_line.backspace();
	} else {
		if (m_cursor->is_at_beginning())
			return;

		auto edit_buffer { get_buffer_type<IEditBuffer>() };
		if (!edit_buffer)
			return;

		if (m_cursor->col == 0) {
			auto deleted_row { m_buffer->line(m_cursor->row) };
			edit_buffer->erase_line(m_cursor->row);
			move_up();
			edit_buffer->append_to(m_cursor->row, deleted_row);
			move_end_line();

			return;
		}

		move_left();
		edit_buffer->erase_char(m_cursor->row, m_cursor->col);
	}
}

void Editor::delete_char() {
	auto edit_buffer { get_buffer_type<IEditBuffer>() };
	if (!edit_buffer)
		return;

	if (m_cursor->col >= static_cast<int>(current_line().size()))
		return;

	edit_buffer->erase_char(m_cursor->row, m_cursor->col);
}

void Editor::newline() {
	if (m_cmd_line.is_active())
		return;

	auto edit_buffer { get_buffer_type<IEditBuffer>() };
	if (!edit_buffer)
		return;

	edit_buffer->insert_newline(m_cursor->row, m_cursor->col);
	m_cursor->col = 0;
	move_down();
}

void Editor::insert_char(char c) {
	if (m_cmd_line.is_active()) {
		m_cmd_line.insert_char(c);
	} else {
		auto edit_buffer { get_buffer_type<IEditBuffer>() };
		if (!edit_buffer)
			return;

		edit_buffer->insert_char(m_cursor->row, m_cursor->col, c);
		move_right();
	}
}

void Editor::move_left() {
	if (m_cmd_line.is_active()) {
		m_cmd_line.move_left();
		return;
	}

	m_cursor->col = std::max(m_cursor->col - 1, 0);
}

void Editor::move_right() {
	if (m_cmd_line.is_active()) {
		m_cmd_line.move_right();
		return;
	}

	int last_valid_column { static_cast<int>(current_line().size()) };
	m_cursor->col = std::min(m_cursor->col + 1, last_valid_column);
}

void Editor::move_up() {
	m_cursor->row = std::max(m_cursor->row - 1, 0);
	m_cursor->col = std::min(m_cursor->col, static_cast<int>(current_line().size()));

	if (m_cursor->row < static_cast<int>(m_top_row)) {
		m_top_row = static_cast<size_t>(m_cursor->row);
	}
}

void Editor::move_down() {
	int last_valid_index { static_cast<int>(m_buffer->line_count() - 1) };
	m_cursor->row = std::min(m_cursor->row + 1, last_valid_index);
	m_cursor->col = std::min(m_cursor->col, static_cast<int>(current_line().size()));

	int visible_rows { std::max<int>(Terminal::get_instance().get_height() - Renderer::BELOW_HEIGHT, 1) };
	if (m_cursor->row >= static_cast<int>(m_top_row) + visible_rows) {
		m_top_row = static_cast<size_t>(m_cursor->row - visible_rows + 1);
	}
}

std::string Editor::current_line() const {
	return std::string(m_buffer->line(m_cursor->row));
}

void Editor::move_end_line() {
	m_cursor->col = current_line().size();
}

void Editor::change_mode(std::unique_ptr<Mode> mode) {
	Terminal::get_instance().set_cursor_shape(mode->get_cursor_shape());
	m_mode = std::move(mode);
}

bool Editor::should_close() const {
	return m_should_close;
}

void Editor::close() { m_should_close = true; }

bool Editor::try_save_to_buffer() {
	auto save_buffer { get_buffer_type<ISaveableBuffer>() };
	if (!save_buffer)
		return false;

	save_buffer->save();
	return true;
}

void Editor::activate_command_line() {
	m_cmd_line.activate();
}

void Editor::deactivate_command_line() {
	m_cmd_line.deactivate();
}

void Editor::exec_and_leave_cmd_line() {
	CommandLineParser parser { m_cmd_line.command() };
	auto result { parser.parse() };

	deactivate_command_line();

	if (result.has_value()) {
		switch (result->type) {
		case CommandType::Write:
			if (try_save_to_buffer())
				m_cmd_line.set_inactive_output("saved buffer \"" + m_buffer->get_name() + "\"");
			else
				m_cmd_line.set_inactive_output(std::format("buffer is not saveable {}", m_buffer->get_name()));
			break;
		case CommandType::Quit:
			m_should_close = true;
			break;

		case CommandType::Open: {
			const auto& file_path_arg { result->args[0] };
			open_path(file_path_arg);

			m_cmd_line.set_inactive_output("succesfully opened \"" + result->args[0] + "\"");
			break;
		}
		case CommandType::OpenExplorer:
			open_buffer(std::make_unique<DirectoryBuffer>(fs::current_path()));

			m_cmd_line.set_inactive_output("");
			break;
		}
	} else {
		m_cmd_line.set_inactive_output(result.error_or(""));
	}
}

void Editor::move_start_line() { m_cursor->col = 0; }

Mode* Editor::get_mode() { return m_mode.get(); }

void Editor::open_path(const fs::path& path) {
	open_buffer(std::make_unique<TextBuffer>(std::make_unique<FileBufferSource>(path)));
}

void Editor::open_buffer(std::unique_ptr<IBuffer> buffer) {
	m_buffer = std::move(buffer);
	m_cursor = &m_buffer->get_cursor();
	m_top_row = 0;
	m_last_key = 0;
}

void Editor::select() {
	auto buffer { get_buffer_type<ISelectableBuffer>() };

	if (buffer)
		buffer->select(*this);
}

IBuffer* Editor::get_active_buffer() { return m_buffer.get(); }
const IBuffer* Editor::get_active_buffer() const { return m_buffer.get(); }

}
