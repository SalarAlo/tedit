#pragma once

#include "Command.h"
#include "Editor.h"

namespace Tedit {
class MoveDownCommand : public ICommand {
public:
	void execute(Editor& e);
};

}
