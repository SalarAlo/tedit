#include <memory>

#include "NormalMode.h"

#include "ChangeToCommandMode.h"
#include "ChangeToModeAction.h"
#include "CommandLine.h"
#include "DeleteCharAction.h"
#include "InsertMode.h"
#include "MoveEndLineAction.h"
#include "MoveLineStartAction.h"
#include "SequenceAction.h"

#include "actions/MoveDownAction.h"
#include "actions/MoveLeftAction.h"
#include "actions/MoveRightAction.h"
#include "actions/MoveUpAction.h"
#include "actions/NewlineAction.h"

namespace Tedit {

std::string NormalMode::get_name() {
	return "normal";
}

std::unique_ptr<IAction> NormalMode::map_action(int key) {
	switch (key) {
	case 'i':
		return std::make_unique<ChangeToModeAction>(std::make_unique<InsertMode>());

	case 'a':
		return sequence(
		    std::make_unique<MoveRightAction>(),
		    std::make_unique<ChangeToModeAction>(std::make_unique<InsertMode>()));

	case 'o':
		return sequence(
		    std::make_unique<MoveEndLineAction>(),
		    std::make_unique<NewlineAction>(),
		    std::make_unique<ChangeToModeAction>(std::make_unique<InsertMode>()));

	case 'O':
		return sequence(
		    std::make_unique<MoveLineStartAction>(),
		    std::make_unique<NewlineAction>(),
		    std::make_unique<MoveUpAction>(),
		    std::make_unique<ChangeToModeAction>(std::make_unique<InsertMode>()));

	case 'h':
		return std::make_unique<MoveLeftAction>();
	case 'j':
		return std::make_unique<MoveDownAction>();
	case 'k':
		return std::make_unique<MoveUpAction>();
	case 'l':
		return std::make_unique<MoveRightAction>();

	case '0':
		return std::make_unique<MoveLineStartAction>();
	case '$':
		return std::make_unique<MoveEndLineAction>();

	case KEY_LEFT:
		return std::make_unique<MoveLeftAction>();
	case KEY_RIGHT:
		return std::make_unique<MoveRightAction>();
	case KEY_UP:
		return std::make_unique<MoveUpAction>();
	case KEY_DOWN:
		return std::make_unique<MoveDownAction>();

	case 'x':
		return std::make_unique<DeleteCharAction>();

	case 's':
		return sequence(
		    std::make_unique<DeleteCharAction>(),
		    std::make_unique<ChangeToModeAction>(std::make_unique<InsertMode>()));

	case CommandLine::COMMAND_LINE_KEY:
		return std::make_unique<ChangeToCommandMode>();

	case '\n':
	case '\r':
		return std::make_unique<MoveDownAction>();

	case KEY_BACKSPACE:
	case 127:
	case 8:
		return std::make_unique<MoveLeftAction>();

	default:
		return nullptr;
	}
}

}
