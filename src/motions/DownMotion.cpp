#include "DownMotion.hpp"

namespace Tedit {

MotionRange DownMotion::resolve(Editor& e) {
	auto* buffer { e.get_buffer() };

	Cursor start { buffer->get_cursor() };
	Cursor end { start };

	if (buffer->line_count() == 0)
		return {
			.start = start,
			.end = start,
			.linewise = true
		};

	size_t last_row { static_cast<size_t>(buffer->line_count() - 1) };

	end.row = std::min<int>(end.row + 1, last_row);

	auto const& target_line { buffer->line(end.row) };

	end.col = std::min<int>(
	    end.col,
	    target_line.size());

	return {
		.start = start,
		.end = end,
		.linewise = true
	};
}

}
