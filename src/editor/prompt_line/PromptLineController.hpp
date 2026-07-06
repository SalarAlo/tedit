#pragma once

#include <memory>
#include <string>
#include <string_view>

#include "IPrompt.hpp"

namespace Tedit {

class Editor;

class PromptLineController {
public:
	void activate(std::unique_ptr<IPrompt> prompt);
	void deactivate();
	void submit(Editor& editor);

	bool is_active() const;
	std::string_view input() const;
	std::string_view inactive_output() const;
	char activation_char() const;
	int cursor_col() const;

	void set_inactive_output(std::string_view output);

	void insert_char(char c);
	void backspace();
	void move_left();
	void move_right();

private:
	void reset_input();

private:
	std::unique_ptr<IPrompt> m_prompt {};

	std::string m_input;
	std::string m_inactive_output;

	int m_cursor_col { 1 };
	bool m_is_active {};
};

}
