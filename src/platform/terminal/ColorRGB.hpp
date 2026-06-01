#pragma once

namespace Tedit {

struct ColorRGB {
	uint8_t r { 0 };
	uint8_t g { 0 };
	uint8_t b { 0 };
};

namespace Colors {
	constexpr ColorRGB BLACK { 0, 0, 0 };
	constexpr ColorRGB WHITE { 255, 255, 255 };
	constexpr ColorRGB RED { 255, 0, 0 };
	constexpr ColorRGB GREEN { 0, 255, 0 };
	constexpr ColorRGB BLUE { 0, 0, 255 };
	constexpr ColorRGB YELLOW { 255, 255, 0 };
	constexpr ColorRGB CYAN { 0, 255, 255 };
	constexpr ColorRGB MAGENTA { 255, 0, 255 };
	constexpr ColorRGB ORANGE { 255, 165, 0 };
	constexpr ColorRGB PURPLE { 128, 0, 128 };
	constexpr ColorRGB PINK { 255, 192, 203 };
	constexpr ColorRGB BROWN { 165, 42, 42 };
	constexpr ColorRGB LIME { 50, 205, 50 };
	constexpr ColorRGB NAVY { 0, 0, 128 };
	constexpr ColorRGB TEAL { 0, 128, 128 };
	constexpr ColorRGB OLIVE { 128, 128, 0 };
	constexpr ColorRGB MAROON { 128, 0, 0 };
	constexpr ColorRGB SILVER { 192, 192, 192 };
	constexpr ColorRGB GRAY { 128, 128, 128 };
	constexpr ColorRGB DARK_GRAY { 64, 64, 64 };
	constexpr ColorRGB LIGHT_GRAY { 211, 211, 211 };
	constexpr ColorRGB GOLD { 255, 215, 0 };
	constexpr ColorRGB BEIGE { 245, 245, 220 };
	constexpr ColorRGB IVORY { 255, 255, 240 };
	constexpr ColorRGB CORAL { 255, 127, 80 };
	constexpr ColorRGB TURQUOISE { 64, 224, 208 };
	constexpr ColorRGB VIOLET { 238, 130, 238 };
}

}
