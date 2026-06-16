#include <catch2/catch_test_macros.hpp>

#include "SearchController.hpp"

TEST_CASE("SearchController stores occurrence length") {
	Tedit::SearchController search_controller {};

	search_controller.submit({ "alpha alphabet" }, "alpha");

	const auto& occurrences { search_controller.get_occurrences() };
	REQUIRE(occurrences.size() == 2);
	REQUIRE(occurrences[0].row == 0);
	REQUIRE(occurrences[0].col == 0);
	REQUIRE(occurrences[0].length == 5);
	REQUIRE(occurrences[1].row == 0);
	REQUIRE(occurrences[1].col == 6);
	REQUIRE(occurrences[1].length == 5);
}

TEST_CASE("SearchController ignores empty searches") {
	Tedit::SearchController search_controller {};

	search_controller.submit({ "alpha" }, "");

	REQUIRE(search_controller.get_occurrences().empty());
}
