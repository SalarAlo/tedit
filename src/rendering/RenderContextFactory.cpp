#include <algorithm>
#include <format>
#include <string>
#include <vector>

#include "RenderContextFactory.hpp"

#include "HighlightKind.hpp"
#include "Renderer.hpp"
#include "Terminal.hpp"
#include "TextBuffer.hpp"

namespace Tedit {

RenderContext RenderContextFactory::create(
    const BufferManager& buffer_manager,
    const PromptLineController& prompt_line,
    SyntaxService& syntax_service,
    const IMode& mode,
    RenderStyle style,
    int top_row) const {
	auto* active_buffer { buffer_manager.get_buffer() };

	int visible_rows { std::max<int>(Terminal::get_instance().get_height() - Renderer::BELOW_HEIGHT - Renderer::ABOVE_HEIGHT, 1) };
	int last_visible_row { std::min<int>(top_row + visible_rows, active_buffer->line_count()) };

	std::vector<RenderLine> visible_lines {};
	for (int row { top_row }; row < last_visible_row; ++row) {
		visible_lines.push_back(RenderLine {
		    .row = row,
		    .text = std::string(active_buffer->line(row)),
		});
	}

	std::vector<BufferTabView> tabs {};
	for (size_t i {}; i < buffer_manager.size(); ++i) {
		tabs.push_back(BufferTabView {
		    .name = buffer_manager.get_buffer(i)->get_name(),
		    .active = i == buffer_manager.get_active_buffer_idx(),
		});
	}

	std::vector<HighlightSpan> highlights {};
	if (auto* text_buffer { dynamic_cast<const TextBuffer*>(active_buffer) })
		highlights = syntax_service.highlight(*text_buffer);

	const auto& search_occurrences { active_buffer->search_controller.get_occurrences() };
	for (const auto& search_occurrence : search_occurrences) {
		highlights.emplace_back(
		    HighlightKind::SearchOccurrence,
		    search_occurrence.col,
		    search_occurrence.col + 1,
		    search_occurrence.row);
	}

	PromptLineView prompt {
		.active = prompt_line.is_active(),
		.text = prompt_line.is_active()
		    ? std::format("{}{}", prompt_line.activation_char(), prompt_line.input())
		    : std::string(prompt_line.inactive_output()),
		.cursor_col = prompt_line.cursor_col(),
	};

	return RenderContext {
		.style = style,
		.top_row = top_row,
		.active_buffer_line_count = active_buffer->line_count(),
		.visible_lines = std::move(visible_lines),
		.tabs = std::move(tabs),
		.highlights = std::move(highlights),
		.active_buffer_name = active_buffer->get_name(),
		.mode_name = mode.get_name(),
		.mode_details = mode.get_mode_details(),
		.active_cursor = active_buffer->get_cursor(),
		.cursor_shape = mode.get_cursor_shape(),
		.prompt = std::move(prompt),
	};
}

}
