#include "MoveMotionAction.hpp"

namespace Tedit {

MoveMotionAction::MoveMotionAction(
    std::unique_ptr<IMotion> motion)
    : m_motion(std::move(motion)) {
}

void MoveMotionAction::execute(Editor& editor) {
	auto range = m_motion->resolve(editor);

	editor.get_active_buffer()->set_cursor(range.end);
}

}
