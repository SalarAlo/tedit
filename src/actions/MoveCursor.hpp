#pragma once

#include "Action.hpp"
#include "Editor.hpp"

namespace Tedit {
class MoveCursor : public IAction {
public:
	MoveCursor(int col, int row);
	void execute(Editor& e);

private:
	int m_row;
	int m_col;
};

}
