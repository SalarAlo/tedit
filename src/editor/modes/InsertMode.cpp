#include "InsertMode.h"

#include "BackspaceAction.h"
#include "ChangeToModeAction.h"
#include "InsertCharAction.h"
#include "MoveDownAction.h"
#include "MoveLeftAction.h"
#include "MoveRightAction.h"
#include "MoveUpAction.h"
#include "NewlineAction.h"
#include "NormalMode.h"

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
		return std::make_unique<MoveLeftAction>();
	case KEY_RIGHT:
		return std::make_unique<MoveRightAction>();
	case KEY_UP:
		return std::make_unique<MoveUpAction>();
	case KEY_DOWN:
		return std::make_unique<MoveDownAction>();
	case 27:
		return std::make_unique<ChangeToModeAction>(std::make_unique<NormalMode>());

	default:
		return std::make_unique<InsertCharAction>(key);
	}
}

}
