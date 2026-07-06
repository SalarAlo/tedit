#include "PromptMode.hpp"

#include "BackspaceAction.hpp"
#include "ExecAndLeavePromptMode.hpp"
#include "InsertCharAction.hpp"
#include "LeavePromptMode.hpp"
#include "MoveMotionAction.hpp"

#include "motions/LeftMotion.hpp"
#include "motions/RightMotion.hpp"

namespace Tedit {

std::unique_ptr<IAction> PromptMode::map_action(Editor&, int key) {
	switch (key) {
	case 27:
		return std::make_unique<LeavePromptMode>();

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
		return std::make_unique<ExecAndLeavePromptMode>();

	default:
		return std::make_unique<InsertCharAction>(key);
	}
}

std::string PromptMode::get_name() const { return "prompt"; }

}
