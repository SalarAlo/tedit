#pragma once

#include "Action.hpp"
#include "CursorShape.hpp"

namespace Tedit {

class Mode {
public:
	virtual ~Mode() = default;
	virtual std::string get_name() = 0;

	virtual std::unique_ptr<IAction> map_action(int key) = 0;

	virtual CursorShape get_cursor_shape();
	virtual std::string get_mode_details();
};

}
