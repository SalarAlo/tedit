#pragma once

#include <optional>

#include "HistoryAction.hpp"

namespace Tedit {

class History {

public:
	void push(const HistoryAction& action);

	std::optional<HistoryAction> pop_undo();

private:
	std::vector<HistoryAction>
	    m_undo_actions {};
};

}
