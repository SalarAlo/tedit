#pragma once

#include "Action.hpp"
#include "Editor.hpp"

namespace Tedit {
class ExecAndLeaveCommandMode : public IAction {
public:
	void execute(Editor& e);
};

}
