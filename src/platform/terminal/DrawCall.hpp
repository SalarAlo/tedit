#pragma once

#include <optional>
#include <string>

#include "ColorRGB.hpp"

namespace Tedit {
struct DrawCall {
	int row;
	int col;
	std::string text {};
	std::optional<ColorRGB> color { std::nullopt };
};

}
