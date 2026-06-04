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
#include "WordMotion.hpp"

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

void insert_text(Tedit::Editor& editor, std::string_view text) {
	for (char c : text) {
		if (c == '\n') {
			editor.newline();
		} else {
			editor.insert_char(c);
		}
	}
}

}

TEST_CASE("Horizontal motions clamp within the current line") {
	Tedit::Editor editor {};
	fill_editor_with_lines(editor);
	editor.get_buffer()->set_cursor({ .row = 0, .col = 0 });

	Tedit::LeftMotion left {};
	auto left_range { left.resolve(editor) };

	REQUIRE(left_range.start.row == 0);
	REQUIRE(left_range.start.col == 0);
	REQUIRE(left_range.end.row == 0);
	REQUIRE(left_range.end.col == 0);

	editor.get_buffer()->set_cursor({ .row = 0, .col = 4 });

	Tedit::RightMotion right {};
	auto right_range { right.resolve(editor) };

	REQUIRE(right_range.start.col == 4);
	REQUIRE(right_range.end.row == 0);
	REQUIRE(right_range.end.col == 4);
}

TEST_CASE("Vertical motions clamp column to target line length") {
	Tedit::Editor editor {};
	fill_editor_with_lines(editor);
	editor.get_buffer()->set_cursor({ .row = 0, .col = 4 });

	Tedit::DownMotion down {};
	auto down_range { down.resolve(editor) };

	REQUIRE(down_range.start.row == 0);
	REQUIRE(down_range.start.col == 4);
	REQUIRE(down_range.end.row == 1);
	REQUIRE(down_range.end.col == 2);
	REQUIRE(down_range.linewise);

	editor.get_buffer()->set_cursor({ .row = 2, .col = 3 });

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
	editor.get_buffer()->set_cursor({ .row = 2, .col = 1 });

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
	editor.get_buffer()->set_cursor({ .row = 1, .col = 1 });

	Tedit::LineMotion motion {};
	auto range { motion.resolve(editor) };

	REQUIRE(range.start.row == 1);
	REQUIRE(range.start.col == 0);
	REQUIRE(range.end.row == 1);
	REQUIRE(range.end.col == 2);
	REQUIRE(range.linewise);
}

TEST_CASE("Word motion moves to the start of the next Vim word") {
	Tedit::Editor editor {};
	insert_text(editor, "alpha, beta\n  gamma.delta");

	Tedit::WordMotion motion {};

	editor.get_buffer()->set_cursor({ .row = 0, .col = 0 });
	auto from_word { motion.resolve(editor) };
	REQUIRE(from_word.end.row == 0);
	REQUIRE(from_word.end.col == 5);

	editor.get_buffer()->set_cursor({ .row = 0, .col = 5 });
	auto from_punctuation { motion.resolve(editor) };
	REQUIRE(from_punctuation.end.row == 0);
	REQUIRE(from_punctuation.end.col == 7);

	editor.get_buffer()->set_cursor({ .row = 0, .col = 7 });
	auto across_line { motion.resolve(editor) };
	REQUIRE(across_line.end.row == 1);
	REQUIRE(across_line.end.col == 2);
}

TEST_CASE("Word end motion moves to the end of the current or next Vim word") {
	Tedit::Editor editor {};
	insert_text(editor, "alpha, beta\n  gamma.delta");

	Tedit::WordEndMotion motion {};

	editor.get_buffer()->set_cursor({ .row = 0, .col = 0 });
	auto from_word_start { motion.resolve(editor) };
	REQUIRE(from_word_start.end.row == 0);
	REQUIRE(from_word_start.end.col == 4);
	REQUIRE(from_word_start.inclusive);

	editor.get_buffer()->set_cursor({ .row = 0, .col = 4 });
	auto from_word_end { motion.resolve(editor) };
	REQUIRE(from_word_end.end.row == 0);
	REQUIRE(from_word_end.end.col == 5);

	editor.get_buffer()->set_cursor({ .row = 0, .col = 6 });
	auto from_space { motion.resolve(editor) };
	REQUIRE(from_space.end.row == 0);
	REQUIRE(from_space.end.col == 10);
}

TEST_CASE("Back word motion moves to the start of the previous Vim word") {
	Tedit::Editor editor {};
	insert_text(editor, "alpha, beta\n  gamma.delta");

	Tedit::BackWordMotion motion {};

	editor.get_buffer()->set_cursor({ .row = 1, .col = 12 });
	auto from_word { motion.resolve(editor) };
	REQUIRE(from_word.end.row == 1);
	REQUIRE(from_word.end.col == 8);

	editor.get_buffer()->set_cursor({ .row = 1, .col = 8 });
	auto from_word_start { motion.resolve(editor) };
	REQUIRE(from_word_start.end.row == 1);
	REQUIRE(from_word_start.end.col == 7);

	editor.get_buffer()->set_cursor({ .row = 1, .col = 2 });
	auto across_line { motion.resolve(editor) };
	REQUIRE(across_line.end.row == 0);
	REQUIRE(across_line.end.col == 7);
}

TEST_CASE("Uppercase WORD motions use whitespace-delimited words") {
	Tedit::Editor editor {};
	insert_text(editor, "alpha, beta\n  gamma.delta");

	Tedit::WordMotion next_word { Tedit::Detail::WordKind::WORD };
	editor.get_buffer()->set_cursor({ .row = 0, .col = 0 });
	auto next_range { next_word.resolve(editor) };
	REQUIRE(next_range.end.row == 0);
	REQUIRE(next_range.end.col == 7);

	Tedit::WordEndMotion end_word { Tedit::Detail::WordKind::WORD };
	editor.get_buffer()->set_cursor({ .row = 0, .col = 0 });
	auto end_range { end_word.resolve(editor) };
	REQUIRE(end_range.end.row == 0);
	REQUIRE(end_range.end.col == 5);

	Tedit::BackWordMotion back_word { Tedit::Detail::WordKind::WORD };
	editor.get_buffer()->set_cursor({ .row = 1, .col = 8 });
	auto back_range { back_word.resolve(editor) };
	REQUIRE(back_range.end.row == 1);
	REQUIRE(back_range.end.col == 2);
}
