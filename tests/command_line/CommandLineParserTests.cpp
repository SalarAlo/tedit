#include <catch2/catch_test_macros.hpp>

#include <string_view>

#include "CommandLineParser.hpp"
#include "CommandType.hpp"

TEST_CASE("CommandLineParser parses write aliases") {
	for (std::string_view command : { "w", "write" }) {
		Tedit::CommandLineParser parser { command };

		auto parsed { parser.parse() };

		REQUIRE(parsed.has_value());
		REQUIRE(parsed->type == Tedit::CommandType::Write);
		REQUIRE(parsed->args.empty());
	}
}

TEST_CASE("CommandLineParser parses quit aliases") {
	for (std::string_view command : { "q", "quit" }) {
		Tedit::CommandLineParser parser { command };

		auto parsed { parser.parse() };

		REQUIRE(parsed.has_value());
		REQUIRE(parsed->type == Tedit::CommandType::QuitCurrentBuffer);
	}
}

TEST_CASE("CommandLineParser parses explorer aliases") {
	for (std::string_view command : { "exp", "explorer" }) {
		Tedit::CommandLineParser parser { command };

		auto parsed { parser.parse() };

		REQUIRE(parsed.has_value());
		REQUIRE(parsed->type == Tedit::CommandType::OpenExplorer);
	}
}

TEST_CASE("CommandLineParser parses open command arguments") {
	Tedit::CommandLineParser parser { "  open   src/Main.cpp  extra" };

	auto parsed { parser.parse() };

	REQUIRE(parsed.has_value());
	REQUIRE(parsed->type == Tedit::CommandType::Open);
	REQUIRE(parsed->args.size() == 2);
	REQUIRE(parsed->args[0] == "src/Main.cpp");
	REQUIRE(parsed->args[1] == "extra");
}

TEST_CASE("CommandLineParser reports invalid commands") {
	{
		Tedit::CommandLineParser parser { "" };
		auto parsed { parser.parse() };

		REQUIRE_FALSE(parsed.has_value());
		REQUIRE(parsed.error() == "empty command");
	}

	{
		Tedit::CommandLineParser parser { "open" };
		auto parsed { parser.parse() };

		REQUIRE_FALSE(parsed.has_value());
		REQUIRE(parsed.error() == "open command requires one argument.");
	}

	{
		Tedit::CommandLineParser parser { "nope" };
		auto parsed { parser.parse() };

		REQUIRE_FALSE(parsed.has_value());
		REQUIRE(parsed.error() == "unknown command: nope");
	}
}
