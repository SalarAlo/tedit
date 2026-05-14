#pragma once

#include "Mode.hpp"

namespace Tedit {
class CommandMode : public Mode {
public:
	std::string get_name() override;
	std::unique_ptr<IAction> map_action(int key) override;
};

}
