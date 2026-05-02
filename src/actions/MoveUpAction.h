#pragma once

#include "Action.h"
#include "Editor.h"

namespace Tedit {
class MoveUpAction : public IAction {
public:
	void execute(Editor& e);
};

}
