#include "DeleteCharAction.h"

namespace Tedit {

void DeleteCharAction::execute(Editor& e) {
		e.delete_char();
	}

}
