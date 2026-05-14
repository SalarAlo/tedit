#pragma once

#include "Action.hpp"

namespace Tedit {
class InsertCharAction : public IAction {
public:
	InsertCharAction(char c);

	void execute(Editor& e) override;

private:
	char m_char;
};

}
