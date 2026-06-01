#include <catch2/catch_test_macros.hpp>

#include <memory>
#include <string_view>

#include "DeleteMotionAction.hpp"
#include "Editor.hpp"

namespace {

class StubMotion final : public Tedit::IMotion {
public:
	explicit StubMotion(Tedit::MotionRange range)
	    : m_range(range) {
	}

	Tedit::MotionRange resolve(Tedit::Editor&) override {
		return m_range;
	}

private:
	Tedit::MotionRange m_range {};
};

void insert_text(Tedit::Editor& editor, std::string_view text) {
	for (char c : text) {
		if (c == '\n') {
			editor.newline();
		} else {
			editor.insert_char(c);
		}
	}
}

Tedit::DeleteMotionAction delete_motion(Tedit::MotionRange range) {
	return Tedit::DeleteMotionAction { std::make_unique<StubMotion>(range) };
}

}

TEST_CASE("DeleteMotionAction deletes an inclusive reversed character range") {
	Tedit::Editor editor {};
	insert_text(editor, "abcde");

	auto action { delete_motion({
	    .start = { .row = 0, .col = 3 },
	    .end = { .row = 0, .col = 1 },
	    .inclusive = true,
	}) };

	action.execute(editor);

	REQUIRE(editor.get_buffer()->line(0) == "ae");
	REQUIRE(editor.get_buffer()->get_cursor().row == 0);
	REQUIRE(editor.get_buffer()->get_cursor().col == 1);
}

TEST_CASE("DeleteMotionAction joins text around a multi-line range") {
	Tedit::Editor editor {};
	insert_text(editor, "abcd\nxy\nuvw");

	auto action { delete_motion({
	    .start = { .row = 0, .col = 2 },
	    .end = { .row = 2, .col = 1 },
	}) };

	action.execute(editor);

	REQUIRE(editor.get_buffer()->line_count() == 1);
	REQUIRE(editor.get_buffer()->line(0) == "abvw");
	REQUIRE(editor.get_buffer()->get_cursor().row == 0);
	REQUIRE(editor.get_buffer()->get_cursor().col == 2);
}

TEST_CASE("DeleteMotionAction linewise deletion leaves a valid empty buffer") {
	Tedit::Editor editor {};
	insert_text(editor, "one\ntwo");

	auto action { delete_motion({
	    .start = { .row = 0, .col = 0 },
	    .end = { .row = 1, .col = 3 },
	    .linewise = true,
	}) };

	action.execute(editor);

	REQUIRE(editor.get_buffer()->line_count() == 1);
	REQUIRE(editor.get_buffer()->line(0) == "");
	REQUIRE(editor.get_buffer()->get_cursor().row == 0);
	REQUIRE(editor.get_buffer()->get_cursor().col == 0);
}
