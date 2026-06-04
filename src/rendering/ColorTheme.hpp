#pragma once

#include "ColorRGB.hpp"
#include "HighlightKind.hpp"

namespace Tedit {

class ColorTheme {
public:
	ColorRGB color_for(HighlightKind kind) const {
		switch (kind) {
		case HighlightKind::Keyword:
			return ColorRGB { 119, 119, 119 }; // #777777

		case HighlightKind::Type:
			return ColorRGB { 119, 119, 119 }; // #777777

		case HighlightKind::Function:
			return ColorRGB { 255, 255, 255 }; // #ffffff

		case HighlightKind::Variable:
			return ColorRGB { 176, 176, 176 }; // #b0b0b0

		case HighlightKind::BuiltinVariable:
			return ColorRGB { 217, 186, 115 }; // #d9ba73, linked to Constant

		case HighlightKind::Constant:
			return ColorRGB { 217, 186, 115 }; // #d9ba73

		case HighlightKind::Module:
			return ColorRGB { 176, 176, 176 }; // #b0b0b0

		case HighlightKind::String:
			return ColorRGB { 255, 255, 255 }; // #ffffff

		case HighlightKind::Comment:
			return ColorRGB { 80, 88, 93 }; // #50585d

		case HighlightKind::Number:
			return ColorRGB { 217, 186, 115 }; // #d9ba73

		case HighlightKind::Operator:
			return ColorRGB { 119, 119, 119 }; // #777777

		case HighlightKind::Punctuation:
			return ColorRGB { 119, 119, 119 }; // Koda links punctuation to Keyword/Delimiter

		case HighlightKind::Property:
			return ColorRGB { 176, 176, 176 }; // #b0b0b0

		case HighlightKind::Parameter:
			return ColorRGB { 176, 176, 176 }; // #b0b0b0

		case HighlightKind::Preprocessor:
			return ColorRGB { 119, 119, 119 }; // Include/Define/PreCondit style

		case HighlightKind::Unknown:
			return ColorRGB { 176, 176, 176 };
		}

		return ColorRGB { 176, 176, 176 };
	}
};

}
