#include <curses.h>
#include <unistd.h>

#include <algorithm>
#include <filesystem>
#include <memory>
#include <string>

#include "Editor.hpp"

#include "CommandLineParser.hpp"
#include "Renderer.hpp"
#include "Terminal.hpp"

#include "buffer/DirectoryBuffer.hpp"
#include "buffer/FileBufferSource.hpp"
#include "buffer/IEditBuffer.hpp"
#include "buffer/IHistoryBuffer.hpp"
#include "buffer/ISaveableBuffer.hpp"
#include "buffer/ISelectableBuffer.hpp"
#include "buffer/MemoryBufferSource.hpp"
#include "buffer/TextBuffer.hpp"

#include "modes/NormalMode.hpp"

namespace Tedit {

namespace {
	std::string join_lines(const IBuffer& buffer, int start_row, int end_row) {
		std::string text {};

		for (int row { start_row }; row <= end_row; ++row) {
			if (row > start_row)
				text += '\n';

			text += buffer.line(row);
		}

		return text;
	}

}

Editor::Editor() {
	m_buffers.push_back(std::make_unique<TextBuffer>(std::make_unique<MemoryBufferSource>()));
	m_cursor = &get_buffer()->get_cursor();
	change_mode(std::make_unique<NormalMode>());
}

void Editor::handle_key(int key) {
	auto action { get_mode()->map_action(key) };
	if (action)
		action->execute(*this);
}

void Editor::backspace() {
	if (m_prompt_line.is_active()) {
		m_prompt_line.backspace();
	} else {
		if (m_cursor->is_at_beginning())
			return;

		auto cursor_before { *m_cursor };

		auto edit_buffer { get_buffer_type<IEditBuffer>() };
		if (!edit_buffer)
			return;

		if (m_cursor->col == 0) {
			auto deleted_row { get_buffer()->line(m_cursor->row) };
			Cursor join_position {
				.row = m_cursor->row - 1,
				.col = static_cast<int>(get_buffer()->line(m_cursor->row - 1).size())
			};
			edit_buffer->erase_line(m_cursor->row);

			edit_buffer->append_to(join_position.row, deleted_row);
			set_cursor(join_position);

			try_push_undo(DeleteAction { .before = cursor_before, .after = *m_cursor, .text = "\n" });
			return;
		}

		move_left();
		std::string deleted_character { edit_buffer->line(m_cursor->row)[m_cursor->col] };
		edit_buffer->erase_char(m_cursor->row, m_cursor->col);
		try_push_undo(DeleteAction { .before = cursor_before, .after = *m_cursor, .text = deleted_character });
	}
}

void Editor::delete_char() {
	auto edit_buffer { get_buffer_type<IEditBuffer>() };
	if (!edit_buffer)
		return;

	auto line_size { static_cast<int>(current_line().size()) };
	if (m_cursor->col > line_size)
		return;

	auto cursor_before { *m_cursor };

	if (m_cursor->col == line_size) {
		if (m_cursor->row >= get_buffer()->line_count() - 1)
			return;

		auto next_line { get_buffer()->line(m_cursor->row + 1) };
		edit_buffer->append_to(m_cursor->row, next_line);
		edit_buffer->erase_line(m_cursor->row + 1);

		try_push_undo(DeleteAction { .before = cursor_before, .after = *m_cursor, .text = "\n" });
		return;
	}

	std::string deleted_character { edit_buffer->line(m_cursor->row)[m_cursor->col] };
	edit_buffer->erase_char(m_cursor->row, m_cursor->col);
	try_push_undo(DeleteAction { .before = cursor_before, .after = *m_cursor, .text = deleted_character });
}

void Editor::delete_range(Cursor start, Cursor end, bool inclusive, bool linewise) {
	auto edit_buffer { get_buffer_type<IEditBuffer>() };
	if (!edit_buffer || get_buffer()->line_count() == 0)
		return;

	auto cursor_before { *m_cursor };

	if (end.row < start.row || (end.row == start.row && end.col < start.col))
		std::swap(start, end);

	int last_row { get_buffer()->line_count() - 1 };
	start.row = std::clamp(start.row, 0, last_row);
	end.row = std::clamp(end.row, 0, last_row);

	if (linewise) {
		auto deleted_text { join_lines(*get_buffer(), start.row, end.row) };
		Cursor undo_position { .row = start.row, .col = 0 };

		if (start.row > 0 && end.row == last_row) {
			undo_position.row = start.row - 1;
			undo_position.col = static_cast<int>(get_buffer()->line(undo_position.row).size());
			deleted_text = '\n' + deleted_text;
		} else if (end.row < last_row) {
			deleted_text += '\n';
		}

		for (int row { end.row }; row >= start.row; --row)
			edit_buffer->erase_line(row);

		if (edit_buffer->line_count() == 0)
			edit_buffer->set_text("\n");

		Cursor cursor_after {
			.row = std::min(start.row, edit_buffer->line_count() - 1),
			.col = 0
		};
		set_cursor(cursor_after);
		try_push_undo(DeleteAction { .before = cursor_before, .after = undo_position, .text = deleted_text });
		return;
	}

	start.col = std::clamp<int>(start.col, 0, get_buffer()->line(start.row).size());
	end.col = std::clamp<int>(end.col + (inclusive ? 1 : 0), 0, get_buffer()->line(end.row).size());

	std::string deleted_text {};
	if (start.row == end.row) {
		if (end.col <= start.col)
			return;

		auto line { get_buffer()->line(start.row) };
		deleted_text = line.substr(start.col, end.col - start.col);

		for (int col { end.col - 1 }; col >= start.col; --col)
			edit_buffer->erase_char(start.row, col);

		set_cursor(start);
		try_push_undo(DeleteAction { .before = cursor_before, .after = start, .text = deleted_text });
		return;
	}

	deleted_text += get_buffer()->line(start.row).substr(start.col);
	deleted_text += '\n';

	for (int row { start.row + 1 }; row < end.row; ++row) {
		deleted_text += get_buffer()->line(row);
		deleted_text += '\n';
	}

	deleted_text += get_buffer()->line(end.row).substr(0, end.col);

	while (static_cast<int>(edit_buffer->line(start.row).size()) > start.col)
		edit_buffer->erase_char(start.row, start.col);

	for (int row { end.row - 1 }; row > start.row; --row)
		edit_buffer->erase_line(row);

	for (int col {}; col < end.col; ++col)
		edit_buffer->erase_char(start.row + 1, 0);

	auto tail { std::string(edit_buffer->line(start.row + 1)) };
	edit_buffer->erase_line(start.row + 1);
	edit_buffer->append_to(start.row, tail);
	set_cursor(start);
	try_push_undo(DeleteAction { .before = cursor_before, .after = start, .text = deleted_text });
}

void Editor::newline() {
	if (m_prompt_line.is_active())
		return;

	auto cursor_before { *m_cursor };
	auto edit_buffer { get_buffer_type<IEditBuffer>() };
	if (!edit_buffer)
		return;

	edit_buffer->insert_newline(m_cursor->row, m_cursor->col);
	m_cursor->col = 0;
	move_down();

	try_push_undo(InsertAction { .before = cursor_before, .after = *m_cursor, .text = "\n" });
}

void Editor::insert_char(char c) {
	if (m_prompt_line.is_active()) {
		m_prompt_line.insert_char(c);
	} else {
		auto edit_buffer { get_buffer_type<IEditBuffer>() };
		if (!edit_buffer)
			return;

		edit_buffer->insert_char(m_cursor->row, m_cursor->col, c);
		auto cursor_before { *m_cursor };
		move_right();

		try_push_undo(InsertAction { .before = cursor_before, .after = *m_cursor, .text = std::string { c } });
	}
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
	m_buffers.erase(m_buffers.begin() + m_buffer_idx);

	if (m_buffers.empty()) {
		m_cursor = nullptr;
		m_top_row = 0;
		m_should_close = true;
		return;
	}

	if (m_buffer_idx >= m_buffers.size())
		m_buffer_idx = m_buffers.size() - 1;

	activate_current_buffer();
}

void Editor::save_buffer() {
	save_buffer(m_buffer_idx);
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
	CommandLineParser parser { input };
	auto result { parser.parse() };

	if (result.has_value()) {
		switch (result->type) {
		case CommandType::Write:
			save_buffer();
			break;
		case CommandType::Quit:
			close();
			break;

		case CommandType::Open: {
			const auto& file_path_arg { result->args[0] };
			open_path(file_path_arg, false);

			m_prompt_line.set_inactive_output("succesfully opened \"" + result->args[0] + "\"");
			break;
		}
		case CommandType::OpenExplorer:
			open_buffer(std::make_unique<DirectoryBuffer>(fs::current_path()));
			m_prompt_line.set_inactive_output("");
			break;
		case CommandType::WriteAll: {
			for (size_t i {}; i < m_buffers.size(); i++)
				save_buffer(i);
			break;
		}
		case CommandType::WriteQuit: {
			save_buffer();
			close();
			break;
		}
		case CommandType::QuitAll: {
			while (!m_buffers.empty())
				close();
			break;
		}
		}
	} else {
		m_prompt_line.set_inactive_output(result.error_or(""));
	}
}

void Editor::move_start_line() { m_cursor->col = 0; }

IMode* Editor::get_mode() { return m_mode.get(); }

void Editor::open_path(const fs::path& path, bool replace) {
	open_buffer(std::make_unique<TextBuffer>(std::make_unique<FileBufferSource>(path)), replace);
}

void Editor::open_buffer(std::unique_ptr<IBuffer> buffer, bool replace) {
	if (replace) {
		m_buffers[m_buffer_idx] = std::move(buffer);
	} else {
		m_buffers.insert(m_buffers.begin() + m_buffer_idx, std::move(buffer));
	}

	activate_current_buffer();
}

void Editor::select() {
	auto buffer { get_buffer_type<ISelectableBuffer>() };

	if (buffer)
		buffer->select(*this);
}

IBuffer* Editor::get_buffer() { return m_buffers[m_buffer_idx].get(); }
const IBuffer* Editor::get_buffer() const { return m_buffers[m_buffer_idx].get(); }

IBuffer* Editor::get_buffer(size_t i) { return m_buffers[i].get(); }
const IBuffer* Editor::get_buffer(size_t i) const { return m_buffers[i].get(); }

void Editor::switch_tab(bool next) {
	if (m_buffers.empty())
		return;

	if (next) {
		m_buffer_idx++;
		if (m_buffer_idx >= m_buffers.size())
			m_buffer_idx = 0;
	} else {
		if (m_buffer_idx == 0)
			m_buffer_idx = m_buffers.size() - 1;
		else
			m_buffer_idx--;
	}

	activate_current_buffer();
}

void Editor::activate_current_buffer() {
	m_cursor = &get_buffer()->get_cursor();
	m_top_row = 0;
	m_last_key = 0;
}

void Editor::try_push_undo(const HistoryAction& action) {
	auto history_buffer { get_buffer_type<IHistoryBuffer>() };
	if (!history_buffer)
		return;
	history_buffer->push_undo(action);
}

void Editor::undo() {
	auto history_buffer { get_buffer_type<IHistoryBuffer>() };
	if (history_buffer)
		history_buffer->undo();
}

SyntaxService& Editor::get_syntax_service() { return m_syntax_service; }

void Editor::execute_search_prompt(std::string_view search) {
	get_buffer()->search_controller.submit(get_buffer()->lines(), search);
}

void Editor::move_to_next_search() { get_buffer()->search_controller.move_to_next(*m_cursor); }

}
