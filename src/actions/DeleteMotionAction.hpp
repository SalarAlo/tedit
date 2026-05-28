#pragma once

#include <algorithm>
#include <string>
#include <utility>

#include "Action.hpp"
#include "IEditBuffer.hpp"
#include "IMotion.hpp"

namespace Tedit {
class DeleteMotionAction : public IAction {
public:
	DeleteMotionAction(std::unique_ptr<IMotion> motion)
	    : m_motion(std::move(motion)) { }

	void execute(Editor& e) override {
		auto* buffer { dynamic_cast<IEditBuffer*>(e.get_active_buffer()) };
		if (!buffer)
			return;

		auto range { m_motion->resolve(e) };
		auto start { range.start };
		auto end { range.end };

		if (end.row < start.row || (end.row == start.row && end.col < start.col))
			std::swap(start, end);

		if (range.linewise) {
			for (int row { end.row }; row >= start.row; --row)
				buffer->erase_line(row);

			if (buffer->line_count() == 0)
				buffer->set_text("\n");

			start.row = std::min(start.row, buffer->line_count() - 1);
			start.col = 0;
			buffer->set_cursor(start);
			return;
		}

		if (range.inclusive)
			end.col++;

		if (start.row == end.row) {
			end.col = std::min<int>(end.col, buffer->line(start.row).size());

			for (int col { end.col - 1 }; col >= start.col; --col)
				buffer->erase_char(start.row, col);

			buffer->set_cursor(start);
			return;
		}

		while (static_cast<int>(buffer->line(start.row).size()) > start.col)
			buffer->erase_char(start.row, start.col);

		for (int row { end.row - 1 }; row > start.row; --row)
			buffer->erase_line(row);

		end.col = std::min<int>(end.col, buffer->line(start.row + 1).size());

		for (int col {}; col < end.col; ++col)
			buffer->erase_char(start.row + 1, 0);

		auto tail { std::string(buffer->line(start.row + 1)) };
		buffer->erase_line(start.row + 1);
		buffer->append_to(start.row, tail);
		buffer->set_cursor(start);
	}

private:
	std::unique_ptr<IMotion> m_motion;
};

}
