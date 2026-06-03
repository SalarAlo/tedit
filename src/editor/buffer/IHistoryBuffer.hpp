#pragma once

#include "IBuffer.hpp"

#include "history/History.hpp"

namespace Tedit {
class IHistoryBuffer : virtual public IBuffer {
public:
	void undo();
	void push_undo(const HistoryAction& action);

protected:
	virtual void handle_undo(const HistoryAction& action) = 0;

private:
	History m_history {};
};

}
