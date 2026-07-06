#include "ExecuteMacroAction.hpp"

namespace Tedit {

ExecuteMacroAction::ExecuteMacroAction(char key)
	    : m_key(key) { }

void ExecuteMacroAction::execute(Editor& e) {
		e.execute_macro(m_key);
	}

}
