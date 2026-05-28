#include <catch2/catch_test_macros.hpp>

#include "Cursor.hpp"

TEST_CASE("Cursor detects beginning of buffer") {
	Tedit::Cursor cursor {};

	REQUIRE(cursor.is_at_beginning());

	cursor.col = 1;
	REQUIRE_FALSE(cursor.is_at_beginning());

	cursor.col = 0;
	cursor.row = 1;
	REQUIRE_FALSE(cursor.is_at_beginning());
}

TEST_CASE("Cursor formats human-readable position") {
	Tedit::Cursor cursor {
		.row = 2,
		.col = 5,
	};

	REQUIRE(cursor.to_string() == "3:5");
}

TEST_CASE("Cursor reset returns to origin") {
	Tedit::Cursor cursor {
		.row = 4,
		.col = 8,
	};

	cursor.reset();

	REQUIRE(cursor.row == 0);
	REQUIRE(cursor.col == 0);
}
