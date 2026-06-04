#pragma once

#include "syntax/HighlightKind.hpp"

namespace Tedit {
struct HighlightSpan {
	HighlightKind kind {};
	int start_col {};
	int past_end_col {};
	int row {};
};

}
