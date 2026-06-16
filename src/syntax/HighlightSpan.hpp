#pragma once

#include <unordered_map>

#include "syntax/HighlightKind.hpp"

namespace Tedit {

struct HighlightSpan {
	HighlightKind kind {};
	int start_col {};
	int past_end_col {};
	int row {};
};

using HighlightsLineMap = std::unordered_map<int, std::vector<HighlightSpan>>;

}
