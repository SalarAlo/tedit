#include "BackspaceAction.hpp"

namespace Tedit {

void BackspaceAction::execute(Editor& e) {
	e.backspace();
}

}
