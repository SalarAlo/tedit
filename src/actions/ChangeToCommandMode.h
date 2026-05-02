#pragma once

#include <memory>

#include "Action.h"
#include "CommandMode.h"
#include "Editor.h"
#include "Terminal.h"

namespace Tedit {
class ChangeToCommandMode : public IAction {
public:
	void execute(Editor& e) override {
		e.activate_command_line();
		e.change_mode(std::make_unique<CommandMode>());
	}
};

}
