#pragma once

#include "Action.hpp"

#include "modes/Mode.hpp"

namespace Tedit {

class NormalMode : public Mode {
public:
	virtual std::string get_name() override;
	std::unique_ptr<IAction> map_action(int key) override;
};

}
