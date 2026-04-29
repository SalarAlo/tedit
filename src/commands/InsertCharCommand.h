#pragma once

#include "Command.h"

namespace Tedit {
class InsertCharCommand : public ICommand {
public:
	InsertCharCommand(char c);

	void execute(Editor& e) override;

private:
	char m_char;
};

}
