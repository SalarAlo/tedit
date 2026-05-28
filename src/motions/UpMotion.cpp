#include "UpMotion.hpp"

namespace Tedit {

MotionRange UpMotion::resolve(Editor& e) {
	auto* buffer { e.get_active_buffer() };

	Cursor start { buffer->get_cursor() };
	Cursor end { start };

	if (buffer->line_count() == 0)
		return {
			.start = start,
			.end = start,
			.linewise = true
		};

	end.row = std::max(end.row - 1, 0);

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
