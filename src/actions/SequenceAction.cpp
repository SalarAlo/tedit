#include "SequenceAction.hpp"

namespace Tedit {

SequenceAction::SequenceAction(std::vector<std::unique_ptr<IAction>> actions)
    : m_actions(std::move(actions)) { }

void SequenceAction::execute(Editor& editor) {
	for (auto& action : m_actions) {
		if (action)
			action->execute(editor);
	}
}

}
