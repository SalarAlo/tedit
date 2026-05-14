#pragma once

#include "Action.hpp"
#include "Editor.hpp"

namespace Tedit {
class MoveRightAction : public IAction {
public:
	void execute(Editor& e);
};

}
