#pragma once

#include "Action.h"

#include "modes/Mode.h"

namespace Tedit {

class InsertMode : public Mode {
public:
	virtual std::string get_name() override;
	std::unique_ptr<IAction> map_action(int key) override;
};

}
