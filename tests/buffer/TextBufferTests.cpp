#include <catch2/catch_test_macros.hpp>

#include <memory>
#include <stdexcept>
#include <string>
#include <string_view>
#include <utility>

#include "IBufferSource.hpp"
#include "TextBuffer.hpp"

namespace {

class RecordingBufferSource : public Tedit::IBufferSource {
public:
	explicit RecordingBufferSource(std::string initial_text = {}, std::string name = "recording")
	    : m_buffer(std::move(initial_text))
	    , m_name(std::move(name)) {
	}

	void write(std::string_view data) override {
		m_buffer = data;
	}

	std::string read() const override {
		return m_buffer;
	}

	std::string get_buffer_name() const override {
		return m_name;
	}

	std::string m_buffer {};
	std::string m_name {};
};

std::unique_ptr<Tedit::TextBuffer> make_buffer(std::string initial_text = {}) {
	return std::make_unique<Tedit::TextBuffer>(
	    std::make_unique<RecordingBufferSource>(std::move(initial_text)));
}

}

TEST_CASE("TextBuffer creates one empty line for empty source") {
	auto buffer { make_buffer() };

	REQUIRE(buffer->line_count() == 1);
	REQUIRE(buffer->line(0) == "");
}

TEST_CASE("TextBuffer loads source text into lines") {
	auto buffer { make_buffer("first\nsecond\nthird") };

	REQUIRE(buffer->line_count() == 3);
	REQUIRE(buffer->line(0) == "first");
	REQUIRE(buffer->line(1) == "second");
	REQUIRE(buffer->line(2) == "third");
}

TEST_CASE("TextBuffer inserts and erases characters at requested positions") {
	auto buffer { make_buffer("abcd") };

	buffer->insert_char(0, 2, 'X');
	REQUIRE(buffer->line(0) == "abXcd");

	buffer->erase_char(0, 2);
	REQUIRE(buffer->line(0) == "abcd");
}

TEST_CASE("TextBuffer splits lines and appends to existing lines") {
	auto buffer { make_buffer("abcdef") };

	buffer->insert_newline(0, 3);
	REQUIRE(buffer->line_count() == 2);
	REQUIRE(buffer->line(0) == "abc");
	REQUIRE(buffer->line(1) == "def");

	buffer->append_to(0, buffer->line(1));
	buffer->erase_line(1);

	REQUIRE(buffer->line_count() == 1);
	REQUIRE(buffer->line(0) == "abcdef");
}

TEST_CASE("TextBuffer save writes all lines with trailing newlines") {
	auto source { std::make_unique<RecordingBufferSource>("alpha\nbeta", "scratch") };
	auto* source_ptr { source.get() };
	Tedit::TextBuffer buffer { std::move(source) };

	buffer.insert_newline(1, 2);
	buffer.save();

	REQUIRE(source_ptr->m_buffer == "alpha\nbe\nta\n");
	REQUIRE(buffer.get_name() == "scratch");
}

TEST_CASE("TextBuffer rejects out-of-range edits") {
	auto buffer { make_buffer("abc") };

	REQUIRE_THROWS_AS(buffer->line(1), std::out_of_range);
	REQUIRE_THROWS_AS(buffer->insert_char(1, 0, 'x'), std::out_of_range);
	REQUIRE_THROWS_AS(buffer->insert_char(0, 4, 'x'), std::out_of_range);
	REQUIRE_THROWS_AS(buffer->erase_char(0, 3), std::out_of_range);
	REQUIRE_THROWS_AS(buffer->insert_newline(0, 4), std::out_of_range);
	REQUIRE_THROWS_AS(buffer->erase_line(1), std::out_of_range);
	REQUIRE_THROWS_AS(buffer->append_to(1, "x"), std::out_of_range);
}
