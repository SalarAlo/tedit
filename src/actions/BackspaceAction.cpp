#include "BackspaceAction.h"

namespace Tedit {

void BackspaceAction::execute(Editor& e) {
	e.backspace();
}

}
