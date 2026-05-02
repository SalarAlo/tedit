#include "ExecAndLeaveCommandMode.h"

#include "NormalMode.h"

namespace Tedit {

void ExecAndLeaveCommandMode::execute(Editor& e) {
	e.parse_and_leave_cmd_line();
	e.change_mode(std::make_unique<NormalMode>());
}

}
