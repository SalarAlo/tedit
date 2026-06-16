#include "ExecAndLeavePromptMode.hpp"

#include "NormalMode.hpp"

namespace Tedit {

void ExecAndLeavePromptMode::execute(Editor& e) {
	e.submit_prompt_line();
	e.deactivate_prompt_line();
	e.change_mode(std::make_unique<NormalMode>());
}

}
