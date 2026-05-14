#include "LeaveCommandMode.hpp"

#include "NormalMode.hpp"

namespace Tedit {

void LeaveCommandMode::execute(Editor& e) {
	e.deactivate_command_line();
	e.change_mode(std::make_unique<NormalMode>());
}

}
