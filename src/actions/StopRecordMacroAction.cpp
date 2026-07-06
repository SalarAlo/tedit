#include "StopRecordMacroAction.hpp"

#include "Editor.hpp"

namespace Tedit {

void StopRecordMacroAction::execute(Editor& e) {
	e.stop_recording_macro();
}

bool StopRecordMacroAction::records_macro_key() const { return false; }

}
