
#pragma once

#include "Action.h"
#include "Editor.h"

namespace Tedit {
class CloseAction : public IAction {
public:
	void execute(Editor& e) override;
};

}
