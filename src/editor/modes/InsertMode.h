#pragma once

#include "Command.h"

#include "modes/Mode.h"

namespace Tedit {

class InsertMode : public Mode {
public:
	virtual std::string get_name() override;
	std::unique_ptr<ICommand> map(int key) override;
};

}
