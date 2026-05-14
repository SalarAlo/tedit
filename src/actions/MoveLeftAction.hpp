#pragma once

#include "Action.hpp"
#include "Editor.hpp"

namespace Tedit {
class MoveLeftAction : public IAction {
public:
	void execute(Editor& e);
};

}
