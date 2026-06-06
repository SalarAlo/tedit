#pragma once

#include "ColorRGB.hpp"
#include "HighlightKind.hpp"

namespace Tedit {

class IColorTheme {
public:
	virtual ColorRGB color_for(HighlightKind kind) const = 0;
};

}
