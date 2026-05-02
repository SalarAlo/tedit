#include "CompositeAction.h"

namespace Tedit {

CompositeAction::CompositeAction(std::vector<std::unique_ptr<IAction>> actions)
    : m_actions(std::move(actions)) { }

void CompositeAction::execute(Editor& e) {
	for (const auto& action : m_actions)
		action->execute(e);
}

}
