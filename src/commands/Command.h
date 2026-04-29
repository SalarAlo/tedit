#pragma once

namespace Tedit {

class Editor;

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
