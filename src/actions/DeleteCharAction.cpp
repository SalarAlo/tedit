#include "DeleteCharAction.hpp"

namespace Tedit {

void DeleteCharAction::execute(Editor& e) {
		e.delete_char();
	}

}
