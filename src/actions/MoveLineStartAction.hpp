#pragma once

#include "Action.hpp"
#include "Editor.hpp"

namespace Tedit {
class MoveLineStartAction : public IAction {
public:
	void execute(Editor& e);
};

}
