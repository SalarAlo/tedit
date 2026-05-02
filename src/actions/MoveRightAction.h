#pragma once

#include "Action.h"
#include "Editor.h"

namespace Tedit {
class MoveRightAction : public IAction {
public:
	void execute(Editor& e);
};

}
