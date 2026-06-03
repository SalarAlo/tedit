#include "IHistoryBuffer.hpp"

namespace Tedit {

void IHistoryBuffer::undo() {
	auto undo_action { m_history.pop_undo() };
	if (!undo_action)
		return;

	handle_undo(*undo_action);
}

void IHistoryBuffer::push_undo(const HistoryAction& action) { m_history.push(action); }

}
