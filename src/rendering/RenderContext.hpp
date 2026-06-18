#pragma once

#include <string>
#include <vector>

#include "Cursor.hpp"
#include "CursorShape.hpp"
#include "HighlightSpan.hpp"

namespace Tedit {

struct RenderLine {
	int row {};
	std::string text {};
};

struct BufferTabView {
	std::string name {};
	bool active {};
};

struct PromptLineView {
	bool active {};
	std::string text {};
	int cursor_col {};
};

struct RenderContext {
	int top_row {};
	int active_buffer_line_count {};

	std::vector<RenderLine> visible_lines {};
	std::vector<BufferTabView> tabs {};
	std::vector<HighlightSpan> highlights {};

	std::string active_buffer_name {};
	std::string mode_name {};
	std::string mode_details {};

	Cursor active_cursor {};
	CursorShape cursor_shape { CursorShape::Block };

	PromptLineView prompt {};
};

}
