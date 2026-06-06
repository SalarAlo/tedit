#pragma once

#include "IColorTheme.hpp"

namespace Tedit {
class KodaColorTheme final : public IColorTheme {
public:
	ColorRGB color_for(HighlightKind kind) const override;

private:
};

}
