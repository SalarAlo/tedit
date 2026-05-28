#include "LineStartMotion.hpp"

namespace Tedit {

MotionRange LineStartMotion::resolve(Editor& e) {
	auto* buffer { e.get_active_buffer() };

	Cursor start { buffer->get_cursor() };
	Cursor end { start };

	end.col = 0;

	return {
		.start = start,
		.end = end
	};
}

}
