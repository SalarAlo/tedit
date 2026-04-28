#include "BackspaceCommand.h"

namespace Tedit {

void BackspaceCommand::execute(Editor& e) {
	e.backspace();
}

}
