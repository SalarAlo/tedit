#pragma once

#include "Action.hpp"
#include "Editor.hpp"
#include "IMode.hpp"

namespace Tedit {

class ChangeToModeAction final : public IAction {
public:
	ChangeToModeAction(std::unique_ptr<IMode> mode);

	virtual void execute(Editor& e);

private:
	std::unique_ptr<IMode> m_mode {};
};

}
