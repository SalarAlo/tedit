#pragma once

#include "Action.hpp"
#include "CursorShape.hpp"

#include "modes/Mode.hpp"

namespace Tedit {

class InsertMode : public Mode {
public:
	virtual std::string get_name() override;
	std::unique_ptr<IAction> map_action(int key) override;

	CursorShape get_cursor_shape() override;
};

}
