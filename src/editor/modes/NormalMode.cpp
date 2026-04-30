#include "NormalMode.h"

#include "ChangeToMode.h"
#include "CloseCommand.h"
#include "InsertMode.h"

#include "commands/MoveDownCommand.h"
#include "commands/MoveLeftCommand.h"
#include "commands/MoveRightCommand.h"
#include "commands/MoveUpCommand.h"
#include "commands/NewlineCommand.h"

namespace Tedit {

std::string NormalMode::get_name() {
	return "normal";
}

std::unique_ptr<ICommand> NormalMode::map(int key) {
	switch (key) {
	case 'a':
		return std::make_unique<ChangeToMode>(std::make_unique<InsertMode>());

	case 'h':
		return std::make_unique<MoveLeftCommand>();
	case 'j':
		return std::make_unique<MoveDownCommand>();
	case 'k':
		return std::make_unique<MoveUpCommand>();
	case 'l':
		return std::make_unique<MoveRightCommand>();

	case KEY_LEFT:
		return std::make_unique<MoveLeftCommand>();
	case KEY_RIGHT:
		return std::make_unique<MoveRightCommand>();
	case KEY_UP:
		return std::make_unique<MoveUpCommand>();
	case KEY_DOWN:
		return std::make_unique<MoveDownCommand>();

	case KEY_BACKSPACE:
	case 127:
	case 8:
		return std::make_unique<MoveLeftCommand>();

	case '\n':
	case '\r':
		return std::make_unique<NewlineCommand>();

	case 'q':
		return std::make_unique<CloseCommand>();

	default:
		return nullptr;
	}
}

}
