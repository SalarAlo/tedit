#pragma once

#include "Action.hpp"
#include "Editor.hpp"

namespace Tedit {
class SwitchTabAction : public IAction {
public:
	void execute(Editor& editor) override { editor.switch_tab(true); }
};

}
