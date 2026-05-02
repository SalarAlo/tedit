#pragma once

#include "CommandType.h"

namespace Tedit {

using ParsedCommand = std::expected<CommandType, std::string>;

}
