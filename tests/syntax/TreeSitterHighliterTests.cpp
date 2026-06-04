#include <catch2/catch_test_macros.hpp>

#include <ranges>
#include <string>

#include "HighlightKind.hpp"
#include "SyntaxLanguage.hpp"
#include "TreeSitterHighliter.hpp"

namespace {

bool has_kind(const std::vector<Tedit::HighlightSpan>& spans, Tedit::HighlightKind kind) {
	return std::ranges::any_of(spans, [kind](const Tedit::HighlightSpan& span) {
		return span.kind == kind;
	});
}

}

TEST_CASE("TreeSitterHighliter highlights common C++ captures") {
	Tedit::TreeSitterHighliter highliter { Tedit::CPP_LANGUAGE };
	std::string source {
		"class Widget {\n"
		"public:\n"
		"\tint value = 42;\n"
		"\tvoid render() { value = value + 1; }\n"
		"};\n"
	};

	auto spans { highliter.highlight(source) };

	REQUIRE(has_kind(spans, Tedit::HighlightKind::Keyword));
	REQUIRE(has_kind(spans, Tedit::HighlightKind::Type));
	REQUIRE(has_kind(spans, Tedit::HighlightKind::Function));
	REQUIRE(has_kind(spans, Tedit::HighlightKind::Variable));
	REQUIRE(has_kind(spans, Tedit::HighlightKind::Number));
}
