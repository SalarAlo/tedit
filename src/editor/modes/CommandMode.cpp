#include "CommandMode.hpp"

#include "BackspaceAction.hpp"
#include "ExecAndLeaveCommandMode.hpp"
#include "InsertCharAction.hpp"
#include "LeaveCommandMode.hpp"
#include "MoveMotionAction.hpp"
#include "motions/LeftMotion.hpp"
#include "motions/RightMotion.hpp"

namespace Tedit {

std::unique_ptr<IAction> CommandMode::map_action(int key) {
	switch (key) {
	case 27:
		return std::make_unique<LeaveCommandMode>();

	case KEY_LEFT:
		return std::make_unique<MoveMotionAction>(std::make_unique<LeftMotion>());
	case KEY_RIGHT:
		return std::make_unique<MoveMotionAction>(std::make_unique<RightMotion>());

	case KEY_BACKSPACE:
	case 127:
	case 8:
		return std::make_unique<BackspaceAction>();

	case '\n':
	case '\r':
		return std::make_unique<ExecAndLeaveCommandMode>();

	default:
		return std::make_unique<InsertCharAction>(key);
	}
}

std::string CommandMode::get_name() { return "command"; }

}
