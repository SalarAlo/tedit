#include "History.hpp"

#include "HistoryActionMerger.hpp"

namespace Tedit {

void History::push(const HistoryAction& action) {
	auto final_action { action };
	if (!m_undo_actions.empty()) {
		auto merged { std::visit(HistoryActionMerger { m_undo_actions.back() }, final_action) };

		if (merged) {
			final_action = *merged;
			m_undo_actions.pop_back();
		}
	}

	m_undo_actions.push_back(final_action);
}

std::optional<HistoryAction> History::pop_undo() {
	if (m_undo_actions.empty())
		return std::nullopt;

	const auto out { m_undo_actions.back() };
	m_undo_actions.pop_back();
	return out;
}

}
