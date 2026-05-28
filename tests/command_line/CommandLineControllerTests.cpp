#include <catch2/catch_test_macros.hpp>

#include "CommandLineController.hpp"

TEST_CASE("CommandLineController activation resets input") {
	Tedit::CommandLineController command_line {};

	command_line.activate();
	command_line.insert_char('w');
	command_line.deactivate();

	REQUIRE_FALSE(command_line.is_active());
	REQUIRE(command_line.command() == "");
	REQUIRE(command_line.cursor_col() == 1);

	command_line.activate();

	REQUIRE(command_line.is_active());
	REQUIRE(command_line.command() == "");
	REQUIRE(command_line.cursor_col() == 1);
}

TEST_CASE("CommandLineController inserts at cursor") {
	Tedit::CommandLineController command_line {};
	command_line.activate();

	command_line.insert_char('a');
	command_line.insert_char('c');
	command_line.move_left();
	command_line.insert_char('b');

	REQUIRE(command_line.command() == "abc");
	REQUIRE(command_line.cursor_col() == 3);
}

TEST_CASE("CommandLineController backspace removes previous character") {
	Tedit::CommandLineController command_line {};
	command_line.activate();

	command_line.insert_char('a');
	command_line.insert_char('b');
	command_line.insert_char('c');
	command_line.move_left();
	command_line.backspace();

	REQUIRE(command_line.command() == "ac");
	REQUIRE(command_line.cursor_col() == 2);
}

TEST_CASE("CommandLineController cursor movement clamps to command bounds") {
	Tedit::CommandLineController command_line {};
	command_line.activate();

	command_line.move_left();
	REQUIRE(command_line.cursor_col() == 1);

	command_line.insert_char('w');
	command_line.move_right();
	command_line.move_right();
	REQUIRE(command_line.cursor_col() == 2);
}

TEST_CASE("CommandLineController keeps inactive output separate from input") {
	Tedit::CommandLineController command_line {};

	command_line.set_inactive_output("saved");
	command_line.activate();
	command_line.insert_char('q');
	command_line.deactivate();

	REQUIRE(command_line.inactive_output() == "saved");
	REQUIRE(command_line.command() == "");
}
