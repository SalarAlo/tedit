#include "LeavePromptMode.hpp"

#include "NormalMode.hpp"

namespace Tedit {

void LeavePromptMode::execute(Editor& e) {
	e.deactivate_prompt_line();
	e.change_mode(std::make_unique<NormalMode>());
}

}
