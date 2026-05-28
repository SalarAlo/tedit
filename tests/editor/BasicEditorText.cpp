#include <catch2/catch_test_macros.hpp>

#include "Editor.hpp"

TEST_CASE("Insert character into empty buffer") {
	Tedit::Editor editor {};

	editor.insert_char('a');

	REQUIRE(editor.get_active_buffer()->line(0) == "a");
	REQUIRE(editor.get_active_buffer()->line_count() == 1uz);
}

TEST_CASE("Insert multiple characters") {
	Tedit::Editor editor {};

	editor.insert_char('a');
	editor.insert_char('b');
	editor.insert_char('c');

	REQUIRE(editor.get_active_buffer()->line(0) == "abc");
	REQUIRE(editor.get_active_buffer()->line_count() == 1uz);
}

TEST_CASE("Insert character in middle of line") {
	Tedit::Editor editor {};

	editor.insert_char('a');
	editor.insert_char('c');

	editor.move_left();
	editor.insert_char('b');

	REQUIRE(editor.get_active_buffer()->line(0) == "abc");
}

TEST_CASE("Backspace removes previous character") {
	Tedit::Editor editor {};

	editor.insert_char('a');
	editor.insert_char('b');
	editor.insert_char('c');

	editor.backspace();

	REQUIRE(editor.get_active_buffer()->line(0) == "ab");
}

TEST_CASE("Backspace in middle of line") {
	Tedit::Editor editor {};

	editor.insert_char('a');
	editor.insert_char('b');
	editor.insert_char('c');

	editor.move_left();

	editor.backspace();

	REQUIRE(editor.get_active_buffer()->line(0) == "ac");
}

TEST_CASE("Backspace at beginning does nothing") {
	Tedit::Editor editor {};

	editor.insert_char('a');

	editor.move_left();
	editor.backspace();

	REQUIRE(editor.get_active_buffer()->line(0) == "a");
}

TEST_CASE("Delete removes character under cursor") {
	Tedit::Editor editor {};

	editor.insert_char('a');
	editor.insert_char('b');
	editor.insert_char('c');

	editor.move_left();
	editor.move_left();

	editor.delete_char();

	REQUIRE(editor.get_active_buffer()->line(0) == "ac");
}

TEST_CASE("Delete at end of line does nothing") {
	Tedit::Editor editor {};

	editor.insert_char('a');
	editor.insert_char('b');

	editor.delete_char();

	REQUIRE(editor.get_active_buffer()->line(0) == "ab");
}

TEST_CASE("Newline creates new empty line") {
	Tedit::Editor editor {};

	editor.insert_char('a');
	editor.insert_char('b');

	editor.newline();

	REQUIRE(editor.get_active_buffer()->line_count() == 2uz);
	REQUIRE(editor.get_active_buffer()->line(0) == "ab");
	REQUIRE(editor.get_active_buffer()->line(1) == "");
}

TEST_CASE("Newline splits line in middle") {
	Tedit::Editor editor {};

	editor.insert_char('a');
	editor.insert_char('b');
	editor.insert_char('c');
	editor.insert_char('d');

	editor.move_left();
	editor.move_left();

	editor.newline();

	REQUIRE(editor.get_active_buffer()->line_count() == 2uz);

	REQUIRE(editor.get_active_buffer()->line(0) == "ab");
	REQUIRE(editor.get_active_buffer()->line(1) == "cd");
}

TEST_CASE("Backspace joins lines") {
	Tedit::Editor editor {};

	editor.insert_char('a');
	editor.insert_char('b');

	editor.newline();

	editor.insert_char('c');
	editor.insert_char('d');

	editor.backspace();
	editor.backspace();

	editor.backspace();

	REQUIRE(editor.get_active_buffer()->line_count() == 1uz);
	REQUIRE(editor.get_active_buffer()->line(0) == "ab");
}

TEST_CASE("Delete joins lines") {
	Tedit::Editor editor {};

	editor.insert_char('a');
	editor.insert_char('b');

	editor.newline();

	editor.insert_char('c');
	editor.insert_char('d');

	editor.move_up();
	editor.move_end_line();

	editor.delete_char();

	REQUIRE(editor.get_active_buffer()->line_count() == 1uz);
	REQUIRE(editor.get_active_buffer()->line(0) == "abcd");
}

TEST_CASE("Move left stops at beginning") {
	Tedit::Editor editor {};

	editor.insert_char('a');

	editor.move_left();
	editor.move_left();
	editor.insert_char('b');

	REQUIRE(editor.get_active_buffer()->line(0) == "ba");
}

TEST_CASE("Move right stops at end") {
	Tedit::Editor editor {};

	editor.insert_char('a');

	editor.move_left();
	editor.move_right();
	editor.move_right();

	editor.insert_char('b');

	REQUIRE(editor.get_active_buffer()->line(0) == "ab");
}

TEST_CASE("Move up and down between lines") {
	Tedit::Editor editor {};

	editor.insert_char('a');
	editor.newline();
	editor.insert_char('b');

	editor.move_up();
	editor.insert_char('x');

	REQUIRE(editor.get_active_buffer()->line(0) == "ax");
	REQUIRE(editor.get_active_buffer()->line(1) == "b");
}

TEST_CASE("Move to start of line") {
	Tedit::Editor editor {};

	editor.insert_char('a');
	editor.insert_char('b');
	editor.insert_char('c');

	editor.move_start_line();
	editor.insert_char('x');

	REQUIRE(editor.get_active_buffer()->line(0) == "xabc");
}

TEST_CASE("Move to end of line") {
	Tedit::Editor editor {};

	editor.insert_char('a');
	editor.insert_char('b');

	editor.move_start_line();
	editor.move_end_line();

	editor.insert_char('c');

	REQUIRE(editor.get_active_buffer()->line(0) == "abc");
}

TEST_CASE("Multiple newlines create multiple lines") {
	Tedit::Editor editor {};

	editor.insert_char('a');

	editor.newline();
	editor.newline();

	editor.insert_char('b');

	REQUIRE(editor.get_active_buffer()->line_count() == 3uz);

	REQUIRE(editor.get_active_buffer()->line(0) == "a");
	REQUIRE(editor.get_active_buffer()->line(1) == "");
	REQUIRE(editor.get_active_buffer()->line(2) == "b");
}

TEST_CASE("Complex editing session") {
	Tedit::Editor editor {};

	editor.insert_char('h');
	editor.insert_char('e');
	editor.insert_char('l');
	editor.insert_char('l');
	editor.insert_char('o');

	editor.newline();

	editor.insert_char('w');
	editor.insert_char('o');
	editor.insert_char('r');
	editor.insert_char('l');
	editor.insert_char('d');

	editor.move_up();
	editor.move_end_line();

	editor.insert_char('!');

	REQUIRE(editor.get_active_buffer()->line_count() == 2uz);

	REQUIRE(editor.get_active_buffer()->line(0) == "hello!");
	REQUIRE(editor.get_active_buffer()->line(1) == "world");
}
