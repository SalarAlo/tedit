#include <catch2/catch_test_macros.hpp>

#include <string_view>

#include "DownMotion.hpp"
#include "Editor.hpp"
#include "LeftMotion.hpp"
#include "LineEndMotion.hpp"
#include "LineMotion.hpp"
#include "LineStartMotion.hpp"
#include "RightMotion.hpp"
#include "UpMotion.hpp"

namespace {

void fill_editor_with_lines(Tedit::Editor& editor) {
	for (char c : std::string_view { "abcd" })
		editor.insert_char(c);

	editor.newline();

	for (char c : std::string_view { "xy" })
		editor.insert_char(c);

	editor.newline();

	for (char c : std::string_view { "uvw" })
		editor.insert_char(c);
}

}

TEST_CASE("Horizontal motions clamp within the current line") {
	Tedit::Editor editor {};
	fill_editor_with_lines(editor);
	editor.get_active_buffer()->set_cursor({ .row = 0, .col = 0 });

	Tedit::LeftMotion left {};
	auto left_range { left.resolve(editor) };

	REQUIRE(left_range.start.row == 0);
	REQUIRE(left_range.start.col == 0);
	REQUIRE(left_range.end.row == 0);
	REQUIRE(left_range.end.col == 0);

	editor.get_active_buffer()->set_cursor({ .row = 0, .col = 4 });

	Tedit::RightMotion right {};
	auto right_range { right.resolve(editor) };

	REQUIRE(right_range.start.col == 4);
	REQUIRE(right_range.end.row == 0);
	REQUIRE(right_range.end.col == 4);
}

TEST_CASE("Vertical motions clamp column to target line length") {
	Tedit::Editor editor {};
	fill_editor_with_lines(editor);
	editor.get_active_buffer()->set_cursor({ .row = 0, .col = 4 });

	Tedit::DownMotion down {};
	auto down_range { down.resolve(editor) };

	REQUIRE(down_range.start.row == 0);
	REQUIRE(down_range.start.col == 4);
	REQUIRE(down_range.end.row == 1);
	REQUIRE(down_range.end.col == 2);
	REQUIRE(down_range.linewise);

	editor.get_active_buffer()->set_cursor({ .row = 2, .col = 3 });

	Tedit::UpMotion up {};
	auto up_range { up.resolve(editor) };

	REQUIRE(up_range.start.row == 2);
	REQUIRE(up_range.start.col == 3);
	REQUIRE(up_range.end.row == 1);
	REQUIRE(up_range.end.col == 2);
	REQUIRE(up_range.linewise);
}

TEST_CASE("Line boundary motions resolve start and end columns") {
	Tedit::Editor editor {};
	fill_editor_with_lines(editor);
	editor.get_active_buffer()->set_cursor({ .row = 2, .col = 1 });

	Tedit::LineStartMotion start_motion {};
	auto start_range { start_motion.resolve(editor) };

	REQUIRE(start_range.start.row == 2);
	REQUIRE(start_range.start.col == 1);
	REQUIRE(start_range.end.row == 2);
	REQUIRE(start_range.end.col == 0);

	Tedit::LineEndMotion end_motion {};
	auto end_range { end_motion.resolve(editor) };

	REQUIRE(end_range.end.row == 2);
	REQUIRE(end_range.end.col == 3);
	REQUIRE(end_range.inclusive);
}

TEST_CASE("Line motion selects the whole current line") {
	Tedit::Editor editor {};
	fill_editor_with_lines(editor);
	editor.get_active_buffer()->set_cursor({ .row = 1, .col = 1 });

	Tedit::LineMotion motion {};
	auto range { motion.resolve(editor) };

	REQUIRE(range.start.row == 1);
	REQUIRE(range.start.col == 0);
	REQUIRE(range.end.row == 1);
	REQUIRE(range.end.col == 2);
	REQUIRE(range.linewise);
}
