#pragma once

#include "Terminal.hpp"

namespace Tedit {

class Cursor {
public:
	int row { 0 };
	int col { 0 };

public:
	void apply_cursor(Terminal& terminal) const;
	bool is_at_beginning() const;
	std::string to_string() const;
	void reset();

	bool operator<=>(const Cursor& other) const = default;
};

}
