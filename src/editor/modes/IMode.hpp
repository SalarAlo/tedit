#pragma once

#include "Action.hpp"
#include "CursorShape.hpp"

namespace Tedit {

class Editor;

class IMode {
public:
	virtual ~IMode() = default;
	virtual std::string get_name() const = 0;

	virtual std::unique_ptr<IAction> map_action(Editor& editor, int key) = 0;

	virtual CursorShape get_cursor_shape() const;
	virtual std::string get_mode_details() const;
};

}
