
#pragma once

#include "Command.h"
#include "Editor.h"

namespace Tedit {
class CloseCommand : public ICommand {
public:
	void execute(Editor& e) override;
};

}
