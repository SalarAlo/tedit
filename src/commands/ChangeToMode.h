#pragma once

#include "Command.h"
#include "Editor.h"
#include "Mode.h"

namespace Tedit {

class ChangeToMode final : public ICommand {
public:
	ChangeToMode(std::unique_ptr<Mode> mode);

	virtual void execute(Editor& e);

private:
	std::unique_ptr<Mode> m_mode {};
};

}
