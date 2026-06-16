#pragma once

#include <memory>

#include "Action.hpp"
#include "Editor.hpp"
#include "IPrompt.hpp"
#include "PromptMode.hpp"

namespace Tedit {
class ChangeToPromptMode : public IAction {
public:
	ChangeToPromptMode(std::unique_ptr<IPrompt> prompt)
	    : m_prompt(std::move(prompt)) { };

	void execute(Editor& e) override {
		e.activate_prompt_line(std::move(m_prompt));
		e.change_mode(std::make_unique<PromptMode>());
	}

private:
	std::unique_ptr<IPrompt> m_prompt;
};

}
