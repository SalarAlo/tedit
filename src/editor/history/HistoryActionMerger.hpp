#pragma once

#include <optional>

#include "HistoryAction.hpp"

namespace Tedit {
struct HistoryActionMerger {
	HistoryActionMerger(HistoryAction before_action)
	    : m_before_action(before_action) { };

	std::optional<HistoryAction> operator()(DeleteAction after_action) {
		DeleteAction* before_action { std::get_if<DeleteAction>(&m_before_action) };

		if (!before_action)
			return std::nullopt;

		if (after_action.before != before_action->after)
			return std::nullopt;

		auto text { before_action->text + after_action.text };
		if (is_before(after_action.after, after_action.before))
			text = after_action.text + before_action->text;

		return DeleteAction {
			.before = before_action->before,
			.after = after_action.after,
			.text = text
		};
	}

	std::optional<HistoryAction> operator()(InsertAction after_action) {
		InsertAction* before_action { std::get_if<InsertAction>(&m_before_action) };

		if (!before_action)
			return std::nullopt;

		if (after_action.before != before_action->after)
			return std::nullopt;

		if (after_action.text == "\n")
			return std::nullopt;

		return InsertAction {
			.before = before_action->before,
			.after = after_action.after,
			.text = before_action->text + after_action.text
		};
	}

private:
	bool is_before(const Cursor& lhs, const Cursor& rhs) const {
		return lhs.row < rhs.row || (lhs.row == rhs.row && lhs.col < rhs.col);
	}

	HistoryAction m_before_action {};
};

}
