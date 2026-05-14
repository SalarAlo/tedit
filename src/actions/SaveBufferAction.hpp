
#pragma once

#include "Action.hpp"
#include "Editor.hpp"

namespace Tedit {
class SaveBufferAction : public IAction {
public:
	void execute(Editor& e) override;
};

}
