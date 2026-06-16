#include "NextCursorSearchAction.hpp"

namespace Tedit {

void NextCursorSearchAction::execute(Editor& e) { e.move_to_next_search(); }

}
