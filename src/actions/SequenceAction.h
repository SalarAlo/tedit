#pragma once

#include "Action.h"

namespace Tedit {

class SequenceAction : public IAction {
public:
	explicit SequenceAction(std::vector<std::unique_ptr<IAction>> actions);

	void execute(Editor& editor) override;

private:
	std::vector<std::unique_ptr<IAction>> m_actions;
};

template <typename... Actions>
std::unique_ptr<SequenceAction> sequence(Actions&&... actions) {
	std::vector<std::unique_ptr<IAction>> vec;
	(vec.push_back(std::move(actions)), ...);
	return std::make_unique<SequenceAction>(std::move(vec));
}

}
