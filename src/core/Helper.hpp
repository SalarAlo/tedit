#pragma once

#include "IBuffer.hpp"

namespace Tedit::Helper {

std::string join_lines(const IBuffer& buffer, int start_row, int end_row);

}
