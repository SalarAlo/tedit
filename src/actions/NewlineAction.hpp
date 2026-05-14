#pragma once

#include "Action.hpp"

namespace Tedit {
class NewlineAction : public IAction {
public:
	void execute(Editor& e) override;
};

}
