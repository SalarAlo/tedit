#pragma once

#include "CommandType.hpp"

namespace Tedit {

struct ParsedCommand {
	CommandType type {};
	std::vector<std::string> args {};
};

}
