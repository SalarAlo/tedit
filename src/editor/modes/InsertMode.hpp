#pragma once

#include "Action.hpp"
#include "CursorShape.hpp"

#include "modes/IMode.hpp"

namespace Tedit {

class InsertMode : public IMode {
public:
	virtual std::string get_name() const override;
	std::unique_ptr<IAction> map_action(Editor& editor, int key) override;

	CursorShape get_cursor_shape() const override;
};

}
