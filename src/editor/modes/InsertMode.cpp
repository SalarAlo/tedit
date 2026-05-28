#include "InsertMode.hpp"

#include "BackspaceAction.hpp"
#include "ChangeToModeAction.hpp"
#include "InsertCharAction.hpp"
#include "MoveMotionAction.hpp"
#include "NewlineAction.hpp"
#include "NormalMode.hpp"
#include "SequenceAction.hpp"
#include "motions/DownMotion.hpp"
#include "motions/LeftMotion.hpp"
#include "motions/RightMotion.hpp"
#include "motions/UpMotion.hpp"

namespace Tedit {

std::string InsertMode::get_name() { return "insert"; }

std::unique_ptr<IAction> InsertMode::map_action(int key) {
	switch (key) {
	case KEY_BACKSPACE:
	case 127:
	case 8:
		return std::make_unique<BackspaceAction>();

	case '\n':
	case '\r':
		return std::make_unique<NewlineAction>();

	case KEY_LEFT:
		return std::make_unique<MoveMotionAction>(std::make_unique<LeftMotion>());
	case KEY_RIGHT:
		return std::make_unique<MoveMotionAction>(std::make_unique<RightMotion>());
	case KEY_UP:
		return std::make_unique<MoveMotionAction>(std::make_unique<UpMotion>());
	case KEY_DOWN:
		return std::make_unique<MoveMotionAction>(std::make_unique<DownMotion>());
	case 27:
		return sequence(
		    std::make_unique<MoveMotionAction>(std::make_unique<LeftMotion>()),
		    std::make_unique<ChangeToModeAction>(std::make_unique<NormalMode>()));

	default:
		return std::make_unique<InsertCharAction>(key);
	}
}

CursorShape InsertMode::get_cursor_shape() { return CursorShape::Beam; }

}
