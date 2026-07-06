#include "EditingController.hpp"

#include <algorithm>
#include <string>

#include "Helper.hpp"
#include "IEditBuffer.hpp"
#include "IHistoryBuffer.hpp"

namespace Tedit {

namespace {

IEditBuffer* editable_buffer(IBuffer& buffer) {
	return dynamic_cast<IEditBuffer*>(&buffer);
}

int line_size(const IBuffer& buffer, int row) {
	return static_cast<int>(buffer.line(row).size());
}

void move_right(IBuffer& buffer) {
	auto& cursor { buffer.get_cursor() };
	cursor.col = std::min(cursor.col + 1, line_size(buffer, cursor.row));
}

}

void EditingController::backspace(IBuffer& buffer) {
	auto edit_buffer { editable_buffer(buffer) };
	auto& cursor { buffer.get_cursor() };

	if (!edit_buffer || cursor.is_at_beginning())
		return;

	auto cursor_before { cursor };

	if (cursor.col == 0) {
		std::string deleted_row { buffer.line(cursor.row) };
		Cursor join_position {
			.row = cursor.row - 1,
			.col = line_size(buffer, cursor.row - 1)
		};

		edit_buffer->erase_line(cursor.row);
		edit_buffer->append_to(join_position.row, deleted_row);
		buffer.set_cursor(join_position);

		try_push_undo(buffer, DeleteAction { .before = cursor_before, .after = buffer.get_cursor(), .text = "\n" });
		return;
	}

	cursor.col = std::max(cursor.col - 1, 0);
	std::string deleted_character { edit_buffer->line(cursor.row)[cursor.col] };
	edit_buffer->erase_char(cursor.row, cursor.col);
	try_push_undo(buffer, DeleteAction { .before = cursor_before, .after = buffer.get_cursor(), .text = deleted_character });
}

void EditingController::delete_char(IBuffer& buffer) {
	auto edit_buffer { editable_buffer(buffer) };
	if (!edit_buffer)
		return;

	auto& cursor { buffer.get_cursor() };
	auto current_line_size { line_size(buffer, cursor.row) };
	if (cursor.col > current_line_size)
		return;

	auto cursor_before { cursor };

	if (cursor.col == current_line_size) {
		if (cursor.row >= buffer.line_count() - 1)
			return;

		auto next_line { buffer.line(cursor.row + 1) };
		edit_buffer->append_to(cursor.row, next_line);
		edit_buffer->erase_line(cursor.row + 1);

		try_push_undo(buffer, DeleteAction { .before = cursor_before, .after = buffer.get_cursor(), .text = "\n" });
		return;
	}

	std::string deleted_character { edit_buffer->line(cursor.row)[cursor.col] };
	edit_buffer->erase_char(cursor.row, cursor.col);
	try_push_undo(buffer, DeleteAction { .before = cursor_before, .after = buffer.get_cursor(), .text = deleted_character });
}

void EditingController::delete_range(IBuffer& buffer, Cursor start, Cursor end, bool inclusive, bool linewise) {
	auto edit_buffer { editable_buffer(buffer) };
	if (!edit_buffer || buffer.line_count() == 0)
		return;

	auto cursor_before { buffer.get_cursor() };

	if (end.row < start.row || (end.row == start.row && end.col < start.col))
		std::swap(start, end);

	int last_row { buffer.line_count() - 1 };
	start.row = std::clamp(start.row, 0, last_row);
	end.row = std::clamp(end.row, 0, last_row);

	if (linewise) {
		auto deleted_text { Helper::join_lines(buffer, start.row, end.row) };
		Cursor undo_position { .row = start.row, .col = 0 };

		if (start.row > 0 && end.row == last_row) {
			undo_position.row = start.row - 1;
			undo_position.col = line_size(buffer, undo_position.row);
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
		buffer.set_cursor(cursor_after);
		try_push_undo(buffer, DeleteAction { .before = cursor_before, .after = undo_position, .text = deleted_text });
		return;
	}

	start.col = std::clamp<int>(start.col, 0, buffer.line(start.row).size());
	end.col = std::clamp<int>(end.col + (inclusive ? 1 : 0), 0, buffer.line(end.row).size());

	std::string deleted_text {};
	if (start.row == end.row) {
		if (end.col <= start.col)
			return;

		auto line { buffer.line(start.row) };
		deleted_text = line.substr(start.col, end.col - start.col);

		for (int col { end.col - 1 }; col >= start.col; --col)
			edit_buffer->erase_char(start.row, col);

		buffer.set_cursor(start);
		try_push_undo(buffer, DeleteAction { .before = cursor_before, .after = start, .text = deleted_text });
		return;
	}

	deleted_text += buffer.line(start.row).substr(start.col);
	deleted_text += '\n';

	for (int row { start.row + 1 }; row < end.row; ++row) {
		deleted_text += buffer.line(row);
		deleted_text += '\n';
	}

	deleted_text += buffer.line(end.row).substr(0, end.col);

	while (static_cast<int>(edit_buffer->line(start.row).size()) > start.col)
		edit_buffer->erase_char(start.row, start.col);

	for (int row { end.row - 1 }; row > start.row; --row)
		edit_buffer->erase_line(row);

	for (int col {}; col < end.col; ++col)
		edit_buffer->erase_char(start.row + 1, 0);

	auto tail { std::string(edit_buffer->line(start.row + 1)) };
	edit_buffer->erase_line(start.row + 1);
	edit_buffer->append_to(start.row, tail);
	buffer.set_cursor(start);
	try_push_undo(buffer, DeleteAction { .before = cursor_before, .after = start, .text = deleted_text });
}

void EditingController::newline(IBuffer& buffer) {
	auto edit_buffer { editable_buffer(buffer) };
	if (!edit_buffer)
		return;

	auto cursor_before { buffer.get_cursor() };
	auto& cursor { buffer.get_cursor() };

	edit_buffer->insert_newline(cursor.row, cursor.col);
	cursor.row++;
	cursor.col = 0;

	try_push_undo(buffer, InsertAction { .before = cursor_before, .after = buffer.get_cursor(), .text = "\n" });
}

void EditingController::insert_char(IBuffer& buffer, char c) {
	auto edit_buffer { editable_buffer(buffer) };
	if (!edit_buffer)
		return;

	auto& cursor { buffer.get_cursor() };
	edit_buffer->insert_char(cursor.row, cursor.col, c);
	auto cursor_before { cursor };
	move_right(buffer);

	try_push_undo(buffer, InsertAction { .before = cursor_before, .after = buffer.get_cursor(), .text = std::string { c } });
}

void EditingController::undo(IBuffer& buffer) {
	auto history_buffer { dynamic_cast<IHistoryBuffer*>(&buffer) };
	if (history_buffer)
		history_buffer->undo();
}

void EditingController::try_push_undo(IBuffer& buffer, const HistoryAction& action) {
	auto history_buffer { dynamic_cast<IHistoryBuffer*>(&buffer) };
	if (history_buffer)
		history_buffer->push_undo(action);
}

}
