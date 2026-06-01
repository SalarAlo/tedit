#include <catch2/catch_test_macros.hpp>

#include <memory>

#include "Editor.hpp"
#include "MemoryBufferSource.hpp"
#include "TextBuffer.hpp"

namespace {

std::unique_ptr<Tedit::TextBuffer> make_buffer() {
	return std::make_unique<Tedit::TextBuffer>(std::make_unique<Tedit::MemoryBufferSource>());
}

void insert_text(Tedit::Editor& editor, std::string_view text) {
	for (auto c : text)
		editor.insert_char(c);
}

}

TEST_CASE("Switching buffers updates the editor cursor to the active buffer") {
	Tedit::Editor editor {};
	insert_text(editor, "first");

	editor.open_buffer(make_buffer(), false);
	insert_text(editor, "second");

	editor.switch_tab(true);
	editor.insert_char('!');

	REQUIRE(editor.get_buffer()->line(0) == "first!");

	editor.switch_tab(false);
	editor.insert_char('?');

	REQUIRE(editor.get_buffer()->line(0) == "second?");
}

TEST_CASE("Opening a buffer without replacing makes it active and keeps the current buffer") {
	Tedit::Editor editor {};
	insert_text(editor, "first");

	editor.open_buffer(make_buffer(), false);
	insert_text(editor, "second");

	REQUIRE(editor.get_buffer()->line(0) == "second");

	editor.switch_tab(true);
	REQUIRE(editor.get_buffer()->line(0) == "first");

	editor.switch_tab(false);
	REQUIRE(editor.get_buffer()->line(0) == "second");
}

TEST_CASE("Replacing the active buffer does not create another tab") {
	Tedit::Editor editor {};
	insert_text(editor, "original");

	editor.open_buffer(make_buffer());
	insert_text(editor, "replacement");

	REQUIRE(editor.get_buffer()->line(0) == "replacement");

	editor.switch_tab(true);
	REQUIRE(editor.get_buffer()->line(0) == "replacement");
}

TEST_CASE("Switching tabs wraps around in both directions") {
	Tedit::Editor editor {};
	insert_text(editor, "first");

	editor.open_buffer(make_buffer(), false);
	insert_text(editor, "second");

	editor.switch_tab(false);
	REQUIRE(editor.get_buffer()->line(0) == "first");

	editor.switch_tab(true);
	REQUIRE(editor.get_buffer()->line(0) == "second");
}

TEST_CASE("Each buffer preserves its own cursor while switching tabs") {
	Tedit::Editor editor {};
	insert_text(editor, "abcd");
	editor.move_left();
	editor.move_left();

	editor.open_buffer(make_buffer(), false);
	insert_text(editor, "xy");
	editor.move_left();

	editor.switch_tab(true);
	editor.insert_char('1');

	REQUIRE(editor.get_buffer()->line(0) == "ab1cd");

	editor.switch_tab(false);
	editor.insert_char('2');

	REQUIRE(editor.get_buffer()->line(0) == "x2y");
}

TEST_CASE("Closing the active buffer updates the cursor to the remaining buffer") {
	Tedit::Editor editor {};
	insert_text(editor, "first");

	editor.open_buffer(make_buffer(), false);
	insert_text(editor, "second");

	editor.close();
	editor.insert_char('!');

	REQUIRE_FALSE(editor.should_close());
	REQUIRE(editor.get_buffer()->line(0) == "first!");
}

TEST_CASE("Closing the last tab activates the previous remaining buffer") {
	Tedit::Editor editor {};
	insert_text(editor, "first");

	editor.open_buffer(make_buffer(), false);
	insert_text(editor, "second");

	editor.switch_tab(true);
	editor.close();
	editor.insert_char('!');

	REQUIRE_FALSE(editor.should_close());
	REQUIRE(editor.get_buffer()->line(0) == "second!");
}

TEST_CASE("Closing a middle tab activates the next remaining buffer") {
	Tedit::Editor editor {};
	insert_text(editor, "first");

	editor.open_buffer(make_buffer(), false);
	insert_text(editor, "second");

	editor.open_buffer(make_buffer(), false);
	insert_text(editor, "third");

	editor.switch_tab(true);
	editor.close();
	editor.insert_char('!');

	REQUIRE_FALSE(editor.should_close());
	REQUIRE(editor.get_buffer()->line(0) == "first!");

	editor.switch_tab(false);
	REQUIRE(editor.get_buffer()->line(0) == "third");
}

TEST_CASE("Closing all buffers only requests editor close after the last buffer") {
	Tedit::Editor editor {};
	insert_text(editor, "first");

	editor.open_buffer(make_buffer(), false);
	insert_text(editor, "second");

	editor.close();

	REQUIRE_FALSE(editor.should_close());
	REQUIRE(editor.get_buffer()->line(0) == "first");

	editor.close();

	REQUIRE(editor.should_close());
}
