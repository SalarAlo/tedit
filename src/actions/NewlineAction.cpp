#include "NewlineAction.hpp"

#include "Editor.hpp"

namespace Tedit {

void NewlineAction::execute(Editor& e) {
	e.newline();
}

}
