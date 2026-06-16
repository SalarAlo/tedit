#include "IMode.hpp"

namespace Tedit {

CursorShape IMode::get_cursor_shape() { return CursorShape::Block; }

std::string IMode::get_mode_details() { return ""; }

}
