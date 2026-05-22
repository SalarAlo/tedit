#pragma once
#include "Action.hpp"
#include "Editor.hpp"

namespace Tedit {

class SelectAction : public IAction {
public:
	void execute(Editor& e) override;
};

}
