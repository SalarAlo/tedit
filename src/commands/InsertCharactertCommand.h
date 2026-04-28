#pragma once

#include "Command.h"

namespace Tedit {
class InsertCharCommand : public ICommand {
public:
	InsertCharCommand(char c)
	    : m_char(c) { }

	void execute(Editor& e) override {
		e.insert_char(m_char);
	}

private:
	char m_char;
};

}
