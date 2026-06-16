#include "KodaColorTheme.hpp"

namespace Tedit {

ColorRGB KodaColorTheme::color_for(HighlightKind kind) const {
	switch (kind) {
	case HighlightKind::Keyword:
	case HighlightKind::KeywordConditional:
	case HighlightKind::KeywordDirective:
	case HighlightKind::KeywordException:
	case HighlightKind::KeywordFunction:
	case HighlightKind::KeywordModifier:
	case HighlightKind::KeywordOperator:
	case HighlightKind::KeywordRepeat:
	case HighlightKind::KeywordReturn:
	case HighlightKind::KeywordStorage:
		return ColorRGB { 119, 119, 119 }; // #777777

	case HighlightKind::Type:
	case HighlightKind::BuiltinType:
		return ColorRGB { 119, 119, 119 }; // #777777

	case HighlightKind::Function:
	case HighlightKind::BuiltinFunction:
	case HighlightKind::Method:
	case HighlightKind::Constructor:
	case HighlightKind::Destructor:
	case HighlightKind::Macro:
		return ColorRGB { 255, 255, 255 }; // #ffffff

	case HighlightKind::Variable:
		return ColorRGB { 176, 176, 176 }; // #b0b0b0

	case HighlightKind::BuiltinVariable:
		return ColorRGB { 217, 186, 115 }; // #d9ba73, linked to Constant

	case HighlightKind::Constant:
	case HighlightKind::BuiltinConstant:
	case HighlightKind::Boolean:
		return ColorRGB { 217, 186, 115 }; // #d9ba73

	case HighlightKind::Module:
		return ColorRGB { 176, 176, 176 }; // #b0b0b0

	case HighlightKind::String:
	case HighlightKind::Character:
	case HighlightKind::SpecialString:
		return ColorRGB { 255, 255, 255 }; // #ffffff

	case HighlightKind::Comment:
		return ColorRGB { 80, 88, 93 }; // #50585d

	case HighlightKind::Number:
	case HighlightKind::Float:
	case HighlightKind::SearchOccurrence:
	case HighlightKind::Active:
		return ColorRGB { 217, 186, 115 }; // #d9ba73

	case HighlightKind::Operator:
		return ColorRGB { 119, 119, 119 }; // #777777

	case HighlightKind::Punctuation:
	case HighlightKind::PunctuationBracket:
	case HighlightKind::PunctuationDelimiter:
		return ColorRGB { 119, 119, 119 }; // Koda links punctuation to Keyword/Delimiter

	case HighlightKind::Property:
	case HighlightKind::Parameter:
	case HighlightKind::Label:
		return ColorRGB { 176, 176, 176 }; // #b0b0b0

	case HighlightKind::Attribute:
	case HighlightKind::Preprocessor:
		return ColorRGB { 119, 119, 119 }; // Include/Define/PreCondit style

	case HighlightKind::Unknown:
		return ColorRGB { 176, 176, 176 };
	}

	return ColorRGB { 176, 176, 176 };
}

}
