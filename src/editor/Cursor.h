#pragma once

#include <cstddef>

#include "Terminal.h"

namespace Tedit {

class Cursor {
public:
	void apply_cursor(Terminal& terminal) const;
	bool is_at_beginning() const;

	int row { 0 };
	int col { 0 };
};

}
