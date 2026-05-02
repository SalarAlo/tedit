#include "MoveLineStartAction.h"

namespace Tedit {

void MoveLineStartAction::execute(Editor& e) {
		e.move_start_line();
	}

}
