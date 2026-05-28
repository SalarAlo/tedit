#include "RightMotion.hpp"

namespace Tedit {

MotionRange RightMotion::resolve(Editor& e) {
	auto* buffer { e.get_active_buffer() };

	Cursor start { buffer->get_cursor() };
	Cursor end { start };

	if (buffer->line_count() == 0)
		return {
			.start = start,
			.end = start
		};

	auto const& current_line { buffer->line(end.row) };
	end.col = std::min<int>(
	    end.col + 1,
	    current_line.size());

	return {
		.start = start,
		.end = end
	};
}

}
