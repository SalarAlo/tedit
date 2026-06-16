#include <catch2/catch_test_macros.hpp>

#include <memory>

#include "CommandPrompt.hpp"
#include "PromptLineController.hpp"

TEST_CASE("PromptLineController activation resets input") {
	Tedit::PromptLineController prompt_line {};

	prompt_line.activate(std::make_unique<Tedit::CommandPrompt>());
	prompt_line.insert_char('w');
	prompt_line.deactivate();

	REQUIRE_FALSE(prompt_line.is_active());
	REQUIRE(prompt_line.input() == "");
	REQUIRE(prompt_line.cursor_col() == 1);

	prompt_line.activate(std::make_unique<Tedit::CommandPrompt>());

	REQUIRE(prompt_line.is_active());
	REQUIRE(prompt_line.input() == "");
	REQUIRE(prompt_line.cursor_col() == 1);
}

TEST_CASE("PromptLineController inserts at cursor") {
	Tedit::PromptLineController prompt_line {};
	prompt_line.activate(std::make_unique<Tedit::CommandPrompt>());

	prompt_line.insert_char('a');
	prompt_line.insert_char('c');
	prompt_line.move_left();
	prompt_line.insert_char('b');

	REQUIRE(prompt_line.input() == "abc");
	REQUIRE(prompt_line.cursor_col() == 3);
}

TEST_CASE("PromptLineController backspace removes previous character") {
	Tedit::PromptLineController prompt_line {};
	prompt_line.activate(std::make_unique<Tedit::CommandPrompt>());

	prompt_line.insert_char('a');
	prompt_line.insert_char('b');
	prompt_line.insert_char('c');
	prompt_line.move_left();
	prompt_line.backspace();

	REQUIRE(prompt_line.input() == "ac");
	REQUIRE(prompt_line.cursor_col() == 2);
}

TEST_CASE("PromptLineController cursor movement clamps to input bounds") {
	Tedit::PromptLineController prompt_line {};
	prompt_line.activate(std::make_unique<Tedit::CommandPrompt>());

	prompt_line.move_left();
	REQUIRE(prompt_line.cursor_col() == 1);

	prompt_line.insert_char('w');
	prompt_line.move_right();
	prompt_line.move_right();
	REQUIRE(prompt_line.cursor_col() == 2);
}

TEST_CASE("PromptLineController keeps inactive output separate from input") {
	Tedit::PromptLineController prompt_line {};

	prompt_line.set_inactive_output("saved");
	prompt_line.activate(std::make_unique<Tedit::CommandPrompt>());
	prompt_line.insert_char('q');
	prompt_line.deactivate();

	REQUIRE(prompt_line.inactive_output() == "saved");
	REQUIRE(prompt_line.input() == "");
}
