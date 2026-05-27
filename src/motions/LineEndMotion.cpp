#include "LineEndMotion.h"

namespace Tedit {

MotionRange LineEndMotion::resolve(Editor& e) {
	auto* buffer { e.get_active_buffer() };

	Cursor start { buffer->get_cursor() };
	Cursor end { start };

	if (buffer->line_count() == 0)
		return {
			.start = start,
			.end = start
		};

	end.col = buffer->line(end.row).size();

	return {
		.start = start,
		.end = end,
		.inclusive = true
	};
}

}
