
#pragma once

#include "Action.h"
#include "Editor.h"

namespace Tedit {
class SaveBufferAction : public IAction {
public:
	void execute(Editor& e) override;
};

}
