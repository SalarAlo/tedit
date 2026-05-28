#include "LeftMotion.hpp"

namespace Tedit {

MotionRange LeftMotion::resolve(Editor& e) {
	auto* buffer { e.get_active_buffer() };

	Cursor start { buffer->get_cursor() };
	Cursor end { start };

	end.col = std::max(end.col - 1, 0);

	return {
		.start = start,
		.end = end
	};
}

}
