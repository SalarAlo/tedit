#pragma once

#include "Command.h"

namespace Tedit::CommandMapper {

std::unique_ptr<ICommand> map(int key);

}
