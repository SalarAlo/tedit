#include "ToggleRecordMacroAction.hpp"

namespace Tedit {

ToggleRecordMacroAction::ToggleRecordMacroAction(int key)
    : m_key(key) { }

void ToggleRecordMacroAction::execute(Editor& e) {
	if (e.is_recording()) {
		e.stop_recording_macro();
		return;
	}

	if (is_macro_register())
		e.start_recording_macro(m_key);
}

constexpr bool ToggleRecordMacroAction::is_macro_register() {
	return m_key >= 'a' && m_key <= 'z';
}

}
