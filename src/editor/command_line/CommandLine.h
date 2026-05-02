#pragma once

#include "Action.h"

namespace Tedit {

struct CommandLine {
	inline static constexpr char COMMAND_LINE_KEY { ':' };

	std::string command;
	std::string inactive_output;
	int cursor_col { 1 };
	bool is_active;
};
}
