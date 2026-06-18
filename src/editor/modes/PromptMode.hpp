#pragma once

#include "IMode.hpp"

namespace Tedit {
class PromptMode : public IMode {
public:
	std::string get_name() const override;
	std::unique_ptr<IAction> map_action(int key) override;
};

}
