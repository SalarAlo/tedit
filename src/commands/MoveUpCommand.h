#pragma once

#include "Command.h"
#include "Editor.h"

namespace Tedit {
class MoveUpCommand : public ICommand {
public:
	void execute(Editor& e);
};

}
