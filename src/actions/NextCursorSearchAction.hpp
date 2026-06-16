#pragma once

#include "Action.hpp"
#include "Editor.hpp"

namespace Tedit {
class NextCursorSearchAction : public IAction {
public:
	void execute(Editor& e) override;

private:
};

}
