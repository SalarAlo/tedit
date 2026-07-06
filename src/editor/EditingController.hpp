#pragma once

#include "Cursor.hpp"
#include "HistoryAction.hpp"
#include "IBuffer.hpp"

namespace Tedit {

class EditingController {
public:
	void backspace(IBuffer& buffer);
	void delete_char(IBuffer& buffer);
	void delete_range(IBuffer& buffer, Cursor start, Cursor end, bool inclusive = false, bool linewise = false);
	void newline(IBuffer& buffer);
	void insert_char(IBuffer& buffer, char c);
	void undo(IBuffer& buffer);

private:
	void try_push_undo(IBuffer& buffer, const HistoryAction& action);
};

}
