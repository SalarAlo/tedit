
#pragma once

#include "Action.hpp"
#include "Editor.hpp"

namespace Tedit {
class CloseAction : public IAction {
public:
	void execute(Editor& e) override;
};

}
