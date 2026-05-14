#pragma once

#include <memory>

#include "Action.hpp"
#include "CommandMode.hpp"
#include "Editor.hpp"
#include "Terminal.hpp"

namespace Tedit {
class ChangeToCommandMode : public IAction {
public:
	void execute(Editor& e) override {
		e.activate_command_line();
		e.change_mode(std::make_unique<CommandMode>());
	}
};

}
