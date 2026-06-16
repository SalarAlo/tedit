#pragma once

#include "Action.hpp"
#include "Editor.hpp"

namespace Tedit {
class ExecAndLeavePromptMode : public IAction {
public:
	void execute(Editor& e);
};

}
