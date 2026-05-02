#pragma once

#include "Action.h"
#include "Editor.h"

namespace Tedit {
class MoveLineStartAction : public IAction {
public:
	void execute(Editor& e);
};

}
