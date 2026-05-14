#pragma once

#include "Terminal.hpp"

namespace Tedit {

class Cursor {
public:
	void apply_cursor(Terminal& terminal) const;
	bool is_at_beginning() const;
	std::string to_string() const;

	int row { 0 };
	int col { 0 };
};

}
