#pragma once

#include "Action.h"

namespace Tedit {
class NewlineAction : public IAction {
public:
	void execute(Editor& e) override;
};

}
