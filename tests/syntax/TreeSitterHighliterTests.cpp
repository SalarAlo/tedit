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

bool has_text_kind(const std::string& source, const std::vector<Tedit::HighlightSpan>& spans, std::string_view text, Tedit::HighlightKind kind) {
	std::vector<std::string_view> lines {};
	size_t start {};

	while (start <= source.size()) {
		size_t end { source.find('\n', start) };
		if (end == std::string::npos) {
			lines.push_back(std::string_view { source }.substr(start));
			break;
		}

		lines.push_back(std::string_view { source }.substr(start, end - start));
		start = end + 1;
	}

	return std::ranges::any_of(spans, [&](const Tedit::HighlightSpan& span) {
		if (span.kind != kind || span.row < 0 || static_cast<size_t>(span.row) >= lines.size())
			return false;

		auto line { lines[static_cast<size_t>(span.row)] };
		if (span.start_col < 0 || span.past_end_col < span.start_col || static_cast<size_t>(span.past_end_col) > line.size())
			return false;

		return line.substr(static_cast<size_t>(span.start_col), static_cast<size_t>(span.past_end_col - span.start_col)) == text;
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
	REQUIRE(has_kind(spans, Tedit::HighlightKind::Method));
	REQUIRE(has_kind(spans, Tedit::HighlightKind::Variable));
	REQUIRE(has_kind(spans, Tedit::HighlightKind::Number));
}

TEST_CASE("TreeSitterHighliter highlights C++ specific captures") {
	Tedit::TreeSitterHighliter highliter { Tedit::CPP_LANGUAGE };
	std::string source {
		"#define FLAG(value) value\n"
		"export module demo.core;\n"
		"template <typename T>\n"
		"concept Renderable = requires(T item) { item.render(); };\n"
		"class Widget {\n"
		"public:\n"
		"\t[[nodiscard]] Widget();\n"
		"\t~Widget();\n"
		"\tbool render(int count = 42, char marker = 'x') const;\n"
		"};\n"
		"bool Widget::render(int count, char marker) const {\n"
		"\tthis->render();\n"
		"\treturn FLAG(count) > 0 && marker != '\\0';\n"
		"}\n"
	};

	auto spans { highliter.highlight(source) };

	REQUIRE(has_text_kind(source, spans, "#define", Tedit::HighlightKind::KeywordDirective));
	REQUIRE(has_text_kind(source, spans, "FLAG", Tedit::HighlightKind::Macro));
	REQUIRE(has_text_kind(source, spans, "export", Tedit::HighlightKind::KeywordDirective));
	REQUIRE(has_text_kind(source, spans, "module", Tedit::HighlightKind::KeywordDirective));
	REQUIRE(has_text_kind(source, spans, "typename", Tedit::HighlightKind::KeywordStorage));
	REQUIRE(has_text_kind(source, spans, "concept", Tedit::HighlightKind::KeywordStorage));
	REQUIRE(has_text_kind(source, spans, "requires", Tedit::HighlightKind::KeywordOperator));
	REQUIRE(has_text_kind(source, spans, "public", Tedit::HighlightKind::Keyword));
	REQUIRE(has_text_kind(source, spans, "nodiscard", Tedit::HighlightKind::Attribute));
	REQUIRE(has_text_kind(source, spans, "bool", Tedit::HighlightKind::BuiltinType));
	REQUIRE(has_text_kind(source, spans, "render", Tedit::HighlightKind::Method));
	REQUIRE(has_text_kind(source, spans, "this", Tedit::HighlightKind::BuiltinVariable));
	REQUIRE(has_text_kind(source, spans, "return", Tedit::HighlightKind::KeywordReturn));
	REQUIRE(has_text_kind(source, spans, "42", Tedit::HighlightKind::Number));
	REQUIRE(has_text_kind(source, spans, "'x'", Tedit::HighlightKind::Character));
	REQUIRE(has_text_kind(source, spans, "&&", Tedit::HighlightKind::Operator));
	REQUIRE(has_text_kind(source, spans, "::", Tedit::HighlightKind::PunctuationDelimiter));
}
