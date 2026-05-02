#pragma once

#include "Action.h"
#include "Editor.h"

namespace Tedit {
class BackspaceAction : public IAction {
public:
	void execute(Editor& e) override;

private:
};

}
