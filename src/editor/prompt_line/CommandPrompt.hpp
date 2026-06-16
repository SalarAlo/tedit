#pragma once

#include "Editor.hpp"
#include "IPrompt.hpp"

namespace Tedit {
class CommandPrompt : public IPrompt {
public:
	char activation_char() const override {
		return ':';
	}

	std::string name() const override {
		return "command";
	}

	void submit(Editor& editor, std::string_view input) override {
		editor.execute_command_prompt(input);
	}

private:
};

}
