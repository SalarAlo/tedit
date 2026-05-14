#pragma once

#include "Action.hpp"
#include "Editor.hpp"

namespace Tedit {
class DeleteCharAction : public IAction {
public:
	void execute(Editor& e);
};

}
