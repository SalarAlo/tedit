#pragma once

#include "Action.h"
#include "Editor.h"
#include "Mode.h"

namespace Tedit {

class ChangeToModeAction final : public IAction {
public:
	ChangeToModeAction(std::unique_ptr<Mode> mode);

	virtual void execute(Editor& e);

private:
	std::unique_ptr<Mode> m_mode {};
};

}
