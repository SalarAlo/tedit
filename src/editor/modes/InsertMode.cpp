#include "InsertMode.h"

#include "BackspaceCommand.h"
#include "ChangeToMode.h"
#include "InsertCharCommand.h"
#include "MoveDownCommand.h"
#include "MoveLeftCommand.h"
#include "MoveRightCommand.h"
#include "MoveUpCommand.h"
#include "NewlineCommand.h"
#include "NormalMode.h"

namespace Tedit {

std::string InsertMode::get_name() { return "insert"; }

std::unique_ptr<ICommand> InsertMode::map(int key) {
	switch (key) {
	case KEY_BACKSPACE:
	case 127:
	case 8:
		return std::make_unique<BackspaceCommand>();

	case '\n':
	case '\r':
		return std::make_unique<NewlineCommand>();

	case KEY_LEFT:
		return std::make_unique<MoveLeftCommand>();
	case KEY_RIGHT:
		return std::make_unique<MoveRightCommand>();
	case KEY_UP:
		return std::make_unique<MoveUpCommand>();
	case KEY_DOWN:
		return std::make_unique<MoveDownCommand>();
	case 27:
		return std::make_unique<ChangeToMode>(std::make_unique<NormalMode>());

	default:
		return std::make_unique<InsertCharCommand>(key);
	}
}

}
