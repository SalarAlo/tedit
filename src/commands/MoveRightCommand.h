#pragma once

#include "Command.h"
#include "Editor.h"

namespace Tedit {
class MoveRightCommand : public ICommand {
public:
	void execute(Editor& e);
};

}
