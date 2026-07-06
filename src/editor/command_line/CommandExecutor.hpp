#pragma once

#include <string_view>

namespace Tedit {
class Editor;
namespace CommandExecutor {
	void execute_command_prompt(Editor& editor, std::string_view input);
};

}
