#include <memory>

#include "NormalMode.h"

#include "ChangeToCommandMode.h"
#include "ChangeToModeAction.h"
#include "CloseAction.h"
#include "CommandLine.h"
#include "CommandMode.h"
#include "CompositeAction.h"
#include "InsertMode.h"
#include "SaveBufferAction.h"

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
	case 'a':
		return std::make_unique<ChangeToModeAction>(std::make_unique<InsertMode>());

	case 'h':
		return std::make_unique<MoveLeftAction>();
	case 'j':
		return std::make_unique<MoveDownAction>();
	case 'k':
		return std::make_unique<MoveUpAction>();
	case 'l':
		return std::make_unique<MoveRightAction>();
	case 's':
		return std::make_unique<SaveBufferAction>();

	case CommandLine::COMMAND_LINE_KEY:
		return std::make_unique<ChangeToCommandMode>();

	case KEY_LEFT:
		return std::make_unique<MoveLeftAction>();
	case KEY_RIGHT:
		return std::make_unique<MoveRightAction>();
	case KEY_UP:
		return std::make_unique<MoveUpAction>();
	case KEY_DOWN:
		return std::make_unique<MoveDownAction>();

	case KEY_BACKSPACE:
	case 127:
	case 8:
		return std::make_unique<MoveLeftAction>();

	case '\n':
	case '\r':
		return std::make_unique<NewlineAction>();

	case 'q':
		return std::make_unique<CloseAction>();

	default:
		return nullptr;
	}
}

}
