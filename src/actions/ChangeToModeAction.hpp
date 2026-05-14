#pragma once

#include "Action.hpp"
#include "Editor.hpp"
#include "Mode.hpp"

namespace Tedit {

class ChangeToModeAction final : public IAction {
public:
	ChangeToModeAction(std::unique_ptr<Mode> mode);

	virtual void execute(Editor& e);

private:
	std::unique_ptr<Mode> m_mode {};
};

}
