#pragma once

#include "Action.h"
#include "Editor.h"
#include "NormalMode.h"

namespace Tedit {
class LeaveCommandMode : public IAction {
public:
	void execute(Editor& e);
};

}
