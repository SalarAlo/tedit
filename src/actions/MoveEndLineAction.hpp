#pragma once

#include "Action.hpp"
#include "Editor.hpp"

namespace Tedit {
class MoveEndLineAction : public IAction {
public:
	void execute(Editor& e);

private:
};

}
