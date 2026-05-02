#include "NewlineAction.h"

#include "Editor.h"

namespace Tedit {

void NewlineAction::execute(Editor& e) {
	e.newline();
}

}
