#pragma once

#include "Command.h"

namespace Tedit {

class Mode {
public:
	virtual std::string get_name() = 0;
	virtual std::unique_ptr<ICommand> map(int key) = 0;
};

}
