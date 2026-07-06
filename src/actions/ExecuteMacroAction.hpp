#pragma once

#include "Action.hpp"
#include "Editor.hpp"

namespace Tedit {
class ExecuteMacroAction : public IAction {
public:
	ExecuteMacroAction(char key);

	void execute(Editor& e) override;

private:
	char m_key {};
};

}
