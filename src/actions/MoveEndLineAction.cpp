#include "MoveEndLineAction.h"

namespace Tedit {

void MoveEndLineAction::execute(Editor& e) {
		e.move_end_line();
	}

}
