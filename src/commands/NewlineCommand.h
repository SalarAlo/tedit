#pragma once

#include "Command.h"

namespace Tedit {
class NewlineCommand : public ICommand {
public:
	void execute(Editor& e) override;
};

}
