#pragma once

#include "Action.h"

namespace Tedit {

class Mode {
public:
	virtual std::string get_name() = 0;
	virtual std::unique_ptr<IAction> map_action(int key) = 0;
};

}
