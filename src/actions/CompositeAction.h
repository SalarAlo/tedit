#pragma once

#include "Action.h"

namespace Tedit {
class CompositeAction : public IAction {
public:
	CompositeAction(std::vector<std::unique_ptr<IAction>> actions);
	void execute(Editor& e) override;

private:
	std::vector<std::unique_ptr<IAction>> m_actions;
};

}
