#pragma once

#include "BufferSource.h"
#include "Editor.h"

namespace Tedit {

enum class Command {
	InsertChar,
	Backspace,
	NewLine,
	MoveLeft,
	MoveRight,
	MoveUp,
	MoveDown,
	Quit,
	None
};

class ICommand {
public:
	virtual ~ICommand() = default;
	virtual void execute(Editor&) = 0;
};

}
