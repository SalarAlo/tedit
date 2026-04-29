#include "NewlineCommand.h"

#include "Editor.h"

namespace Tedit {

void NewlineCommand::execute(Editor& e) {
	e.newline();
}

}
