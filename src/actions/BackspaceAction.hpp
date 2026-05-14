#pragma once

#include "Action.hpp"
#include "Editor.hpp"

namespace Tedit {
class BackspaceAction : public IAction {
public:
	void execute(Editor& e) override;

private:
};

}
