#pragma once

#include "Action.hpp"
#include "Editor.hpp"

namespace Tedit {
class LeavePromptMode : public IAction {
public:
	void execute(Editor& e);
};

}
