#include <catch2/catch_test_macros.hpp>

#include "Editor.hpp"
#include "IEditBuffer.hpp"

TEST_CASE("Insert character into empty buffer") {
	Tedit::Editor editor {};

	editor.insert_char('a');

	REQUIRE(editor.get_buffer()->line(0) == "a");
	REQUIRE(editor.get_buffer()->line_count() == 1uz);
}

TEST_CASE("Insert multiple characters") {
	Tedit::Editor editor {};

	editor.insert_char('a');
	editor.insert_char('b');
	editor.insert_char('c');

	REQUIRE(editor.get_buffer()->line(0) == "abc");
	REQUIRE(editor.get_buffer()->line_count() == 1uz);
}

TEST_CASE("Insert character in middle of line") {
	Tedit::Editor editor {};

	editor.insert_char('a');
	editor.insert_char('c');

	editor.move_left();
	editor.insert_char('b');

	REQUIRE(editor.get_buffer()->line(0) == "abc");
}

TEST_CASE("Backspace removes previous character") {
	Tedit::Editor editor {};

	editor.insert_char('a');
	editor.insert_char('b');
	editor.insert_char('c');

	editor.backspace();

	REQUIRE(editor.get_buffer()->line(0) == "ab");
}

TEST_CASE("Backspace in middle of line") {
	Tedit::Editor editor {};

	editor.insert_char('a');
	editor.insert_char('b');
	editor.insert_char('c');

	editor.move_left();

	editor.backspace();

	REQUIRE(editor.get_buffer()->line(0) == "ac");
}

TEST_CASE("Backspace at beginning does nothing") {
	Tedit::Editor editor {};

	editor.insert_char('a');

	editor.move_left();
	editor.backspace();

	REQUIRE(editor.get_buffer()->line(0) == "a");
}

TEST_CASE("Delete removes character under cursor") {
	Tedit::Editor editor {};

	editor.insert_char('a');
	editor.insert_char('b');
	editor.insert_char('c');

	editor.move_left();
	editor.move_left();

	editor.delete_char();

	REQUIRE(editor.get_buffer()->line(0) == "ac");
}

TEST_CASE("Delete at end of line does nothing") {
	Tedit::Editor editor {};

	editor.insert_char('a');
	editor.insert_char('b');

	editor.delete_char();

	REQUIRE(editor.get_buffer()->line(0) == "ab");
}

TEST_CASE("Newline creates new empty line") {
	Tedit::Editor editor {};

	editor.insert_char('a');
	editor.insert_char('b');

	editor.newline();

	REQUIRE(editor.get_buffer()->line_count() == 2uz);
	REQUIRE(editor.get_buffer()->line(0) == "ab");
	REQUIRE(editor.get_buffer()->line(1) == "");
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

	REQUIRE(editor.get_buffer()->line_count() == 2uz);

	REQUIRE(editor.get_buffer()->line(0) == "ab");
	REQUIRE(editor.get_buffer()->line(1) == "cd");
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

	REQUIRE(editor.get_buffer()->line_count() == 1uz);
	REQUIRE(editor.get_buffer()->line(0) == "ab");
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

	REQUIRE(editor.get_buffer()->line_count() == 1uz);
	REQUIRE(editor.get_buffer()->line(0) == "abcd");
}

TEST_CASE("Move left stops at beginning") {
	Tedit::Editor editor {};

	editor.insert_char('a');

	editor.move_left();
	editor.move_left();
	editor.insert_char('b');

	REQUIRE(editor.get_buffer()->line(0) == "ba");
}

TEST_CASE("Move right stops at end") {
	Tedit::Editor editor {};

	editor.insert_char('a');

	editor.move_left();
	editor.move_right();
	editor.move_right();

	editor.insert_char('b');

	REQUIRE(editor.get_buffer()->line(0) == "ab");
}

TEST_CASE("Move up and down between lines") {
	Tedit::Editor editor {};

	editor.insert_char('a');
	editor.newline();
	editor.insert_char('b');

	editor.move_up();
	editor.insert_char('x');

	REQUIRE(editor.get_buffer()->line(0) == "ax");
	REQUIRE(editor.get_buffer()->line(1) == "b");
}

TEST_CASE("Move to start of line") {
	Tedit::Editor editor {};

	editor.insert_char('a');
	editor.insert_char('b');
	editor.insert_char('c');

	editor.move_start_line();
	editor.insert_char('x');

	REQUIRE(editor.get_buffer()->line(0) == "xabc");
}

TEST_CASE("Move to end of line") {
	Tedit::Editor editor {};

	editor.insert_char('a');
	editor.insert_char('b');

	editor.move_start_line();
	editor.move_end_line();

	editor.insert_char('c');

	REQUIRE(editor.get_buffer()->line(0) == "abc");
}

TEST_CASE("Multiple newlines create multiple lines") {
	Tedit::Editor editor {};

	editor.insert_char('a');

	editor.newline();
	editor.newline();

	editor.insert_char('b');

	REQUIRE(editor.get_buffer()->line_count() == 3uz);

	REQUIRE(editor.get_buffer()->line(0) == "a");
	REQUIRE(editor.get_buffer()->line(1) == "");
	REQUIRE(editor.get_buffer()->line(2) == "b");
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

	REQUIRE(editor.get_buffer()->line_count() == 2uz);

	REQUIRE(editor.get_buffer()->line(0) == "hello!");
	REQUIRE(editor.get_buffer()->line(1) == "world");
}

TEST_CASE("Undo removes merged inserted text") {
	Tedit::Editor editor {};

	editor.insert_char('a');
	editor.insert_char('b');
	editor.insert_char('c');

	editor.undo();

	REQUIRE(editor.get_buffer()->line_count() == 1uz);
	REQUIRE(editor.get_buffer()->line(0) == "");
	REQUIRE(editor.get_buffer()->get_cursor().row == 0);
	REQUIRE(editor.get_buffer()->get_cursor().col == 0);
}

TEST_CASE("Undo restores newline insertion") {
	Tedit::Editor editor {};
	auto* edit_buffer { dynamic_cast<Tedit::IEditBuffer*>(editor.get_buffer()) };

	REQUIRE(edit_buffer != nullptr);
	edit_buffer->set_text("ab");
	editor.set_cursor({ .row = 0, .col = 2 });
	editor.newline();

	editor.undo();

	REQUIRE(editor.get_buffer()->line_count() == 1uz);
	REQUIRE(editor.get_buffer()->line(0) == "ab");
	REQUIRE(editor.get_buffer()->get_cursor().row == 0);
	REQUIRE(editor.get_buffer()->get_cursor().col == 2);
}

TEST_CASE("Undo restores line joined by backspace") {
	Tedit::Editor editor {};
	auto* edit_buffer { dynamic_cast<Tedit::IEditBuffer*>(editor.get_buffer()) };

	REQUIRE(edit_buffer != nullptr);
	edit_buffer->set_text("ab\ncd");
	editor.set_cursor({ .row = 1, .col = 0 });
	editor.backspace();

	REQUIRE(editor.get_buffer()->line_count() == 1uz);
	REQUIRE(editor.get_buffer()->line(0) == "abcd");

	editor.undo();

	REQUIRE(editor.get_buffer()->line_count() == 2uz);
	REQUIRE(editor.get_buffer()->line(0) == "ab");
	REQUIRE(editor.get_buffer()->line(1) == "cd");
	REQUIRE(editor.get_buffer()->get_cursor().row == 1);
	REQUIRE(editor.get_buffer()->get_cursor().col == 0);
}

TEST_CASE("Undo repeated backspace preserves deleted text order") {
	Tedit::Editor editor {};
	auto* edit_buffer { dynamic_cast<Tedit::IEditBuffer*>(editor.get_buffer()) };

	REQUIRE(edit_buffer != nullptr);
	edit_buffer->set_text("abcd");
	editor.set_cursor({ .row = 0, .col = 4 });
	editor.backspace();
	editor.backspace();

	REQUIRE(editor.get_buffer()->line(0) == "ab");

	editor.undo();

	REQUIRE(editor.get_buffer()->line(0) == "abcd");
	REQUIRE(editor.get_buffer()->get_cursor().row == 0);
	REQUIRE(editor.get_buffer()->get_cursor().col == 4);
}

TEST_CASE("Undo repeated delete preserves deleted text order") {
	Tedit::Editor editor {};
	auto* edit_buffer { dynamic_cast<Tedit::IEditBuffer*>(editor.get_buffer()) };

	REQUIRE(edit_buffer != nullptr);
	edit_buffer->set_text("abcd");
	editor.set_cursor({ .row = 0, .col = 1 });

	editor.delete_char();
	editor.delete_char();

	REQUIRE(editor.get_buffer()->line(0) == "ad");

	editor.undo();

	REQUIRE(editor.get_buffer()->line(0) == "abcd");
	REQUIRE(editor.get_buffer()->get_cursor().row == 0);
	REQUIRE(editor.get_buffer()->get_cursor().col == 1);
}

TEST_CASE("Recorded macro replays editor input") {
	Tedit::Editor editor {};

	editor.handle_key('q');
	editor.handle_key('a');
	editor.handle_key('i');
	editor.handle_key('x');
	editor.handle_key(27);
	editor.handle_key('q');

	editor.handle_key('@');
	editor.handle_key('a');

	REQUIRE(editor.get_buffer()->line(0) == "xx");
}

TEST_CASE("Stopping macro recording does not leave register input pending") {
	Tedit::Editor editor {};

	editor.handle_key('q');
	editor.handle_key('a');
	editor.handle_key('i');
	editor.handle_key('x');
	editor.handle_key(27);
	editor.handle_key('q');

	editor.handle_key('x');
	editor.handle_key('@');
	editor.handle_key('a');

	REQUIRE(editor.get_buffer()->line(0) == "x");
}

TEST_CASE("Macro stop key is not recorded into replayed input") {
	Tedit::Editor editor {};

	editor.handle_key('q');
	editor.handle_key('a');
	editor.handle_key('i');
	editor.handle_key('x');
	editor.handle_key(27);
	editor.handle_key('q');

	editor.handle_key('@');
	editor.handle_key('a');
	editor.handle_key('i');
	editor.handle_key('y');
	editor.handle_key(27);

	REQUIRE(editor.get_buffer()->line(0) == "yxx");
}

TEST_CASE("Macro records literal q while in insert mode") {
	Tedit::Editor editor {};

	editor.handle_key('q');
	editor.handle_key('a');
	editor.handle_key('i');
	editor.handle_key('q');
	editor.handle_key(27);
	editor.handle_key('q');

	editor.handle_key('@');
	editor.handle_key('a');

	REQUIRE(editor.get_buffer()->line(0) == "qq");
}

TEST_CASE("Invalid macro register does not start recording") {
	Tedit::Editor editor {};

	editor.handle_key('q');
	editor.handle_key('1');
	editor.handle_key('i');
	editor.handle_key('x');
	editor.handle_key(27);

	editor.handle_key('@');
	editor.handle_key('1');

	REQUIRE(editor.get_buffer()->line(0) == "x");
}

TEST_CASE("Macro replay does not record into active macro") {
	Tedit::Editor editor {};

	editor.handle_key('q');
	editor.handle_key('a');
	editor.handle_key('i');
	editor.handle_key('x');
	editor.handle_key(27);
	editor.handle_key('q');

	editor.handle_key('q');
	editor.handle_key('b');
	editor.handle_key('@');
	editor.handle_key('a');
	editor.handle_key('q');

	editor.handle_key('@');
	editor.handle_key('b');

	REQUIRE(editor.get_buffer()->line(0) == "xxx");
}
