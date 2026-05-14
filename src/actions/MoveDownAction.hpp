#pragma once

#include "Action.hpp"
#include "Editor.hpp"

namespace Tedit {
class MoveDownAction : public IAction {
public:
	void execute(Editor& e);
};

}
