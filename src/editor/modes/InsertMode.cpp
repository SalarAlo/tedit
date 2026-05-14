#include "InsertMode.hpp"

#include "BackspaceAction.hpp"
#include "ChangeToModeAction.hpp"
#include "InsertCharAction.hpp"
#include "MoveDownAction.hpp"
#include "MoveLeftAction.hpp"
#include "MoveRightAction.hpp"
#include "MoveUpAction.hpp"
#include "NewlineAction.hpp"
#include "NormalMode.hpp"
#include "SequenceAction.hpp"

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
		return sequence(std::make_unique<MoveLeftAction>(), std::make_unique<ChangeToModeAction>(std::make_unique<NormalMode>()));

	default:
		return std::make_unique<InsertCharAction>(key);
	}
}

CursorShape InsertMode::get_cursor_shape() { return CursorShape::Beam; }

}
