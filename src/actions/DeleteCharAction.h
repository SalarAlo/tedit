#pragma once

#include "Action.h"
#include "Editor.h"

namespace Tedit {
class DeleteCharAction : public IAction {
public:
	void execute(Editor& e);
};

}
