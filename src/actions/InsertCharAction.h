#pragma once

#include "Action.h"

namespace Tedit {
class InsertCharAction : public IAction {
public:
	InsertCharAction(char c);

	void execute(Editor& e) override;

private:
	char m_char;
};

}
