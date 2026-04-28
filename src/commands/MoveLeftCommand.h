#pragma once

#include "Command.h"
#include "Editor.h"

namespace Tedit {
class MoveLeftCommand : public ICommand {
public:
	void execute(Editor& e);
};

}
