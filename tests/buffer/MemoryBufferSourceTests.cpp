#include <catch2/catch_test_macros.hpp>

#include "MemoryBufferSource.hpp"

TEST_CASE("MemoryBufferSource starts empty and stores written data") {
	Tedit::MemoryBufferSource source {};

	REQUIRE(source.read() == "");

	source.write("hello\nworld");

	REQUIRE(source.read() == "hello\nworld");
	REQUIRE(source.get_buffer_name() == "memory_buffer");
}

TEST_CASE("MemoryBufferSource overwrites previous data") {
	Tedit::MemoryBufferSource source {};

	source.write("first");
	source.write("second");

	REQUIRE(source.read() == "second");
}
