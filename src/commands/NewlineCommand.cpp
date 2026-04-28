#include "NewlineCommand.h"

namespace Tedit {

void NewlineCommand::execute(Editor& e) {
	e.newline();
}

}
