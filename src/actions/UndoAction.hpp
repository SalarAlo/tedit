#pragma once

#include "Action.hpp"
#include "Editor.hpp"

namespace Tedit {
class UndoAction : public IAction {
public:
	void execute(Editor& editor) override { editor.undo(); }
};

}
