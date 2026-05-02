#pragma once

#include "Action.h"
#include "Editor.h"

namespace Tedit {
class MoveEndLineAction : public IAction {
public:
	void execute(Editor& e);

private:
};

}
