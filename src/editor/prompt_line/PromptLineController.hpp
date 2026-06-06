#pragma once

#include <string>
#include <string_view>

namespace Tedit {

class PromptLineController {
public:
	inline static constexpr char COMMAND_PROMPT_KEY { ':' };

	void activate();
	void deactivate();

	bool is_active() const;
	std::string_view input() const;
	std::string_view inactive_output() const;
	int cursor_col() const;

	void set_inactive_output(std::string output);

	void insert_char(char c);
	void backspace();
	void move_left();
	void move_right();

private:
	void reset_input();

private:
	std::string m_input;
	std::string m_inactive_output;
	int m_cursor_col { 1 };
	bool m_is_active {};
};

}
