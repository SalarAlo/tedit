#include "LineMotion.hpp"

namespace Tedit {

MotionRange LineMotion::resolve(Editor& e) {
	auto* buffer { e.get_buffer() };

	Cursor cursor { buffer->get_cursor() };

	Cursor start {
		.row = cursor.row,
		.col = 0
	};

	Cursor end {
		.row = cursor.row,
		.col = static_cast<int>(
		    buffer->line(cursor.row).size())
	};

	return {
		.start = start,
		.end = end,
		.linewise = true
	};
}

}
