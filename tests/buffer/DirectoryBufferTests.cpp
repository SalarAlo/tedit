#include <catch2/catch_test_macros.hpp>

#include <chrono>
#include <filesystem>
#include <fstream>
#include <string>

#include "DirectoryBuffer.hpp"
#include "Editor.hpp"

namespace fs = std::filesystem;

namespace {

class TemporaryDirectory {
public:
	TemporaryDirectory()
	    : m_path(fs::temp_directory_path() / ("tedit_directory_buffer_tests_" + std::to_string(std::chrono::steady_clock::now().time_since_epoch().count()))) {
		fs::create_directories(m_path);
	}

	~TemporaryDirectory() {
		std::error_code error {};
		fs::remove_all(m_path, error);
	}

	const fs::path& path() const {
		return m_path;
	}

private:
	fs::path m_path {};
};

int find_line(const Tedit::DirectoryBuffer& buffer, std::string_view line) {
	for (int row {}; row < buffer.line_count(); ++row) {
		if (buffer.line(row) == line)
			return row;
	}

	return -1;
}

}

TEST_CASE("DirectoryBuffer lists parent entry and marks directories") {
	TemporaryDirectory temp {};
	fs::create_directories(temp.path() / "child");
	std::ofstream { temp.path() / "note.txt" } << "hello";

	Tedit::DirectoryBuffer buffer { temp.path() };

	REQUIRE(buffer.get_name() == temp.path().filename().string());
	REQUIRE(buffer.line(0) == "../");
	REQUIRE(find_line(buffer, "child/") >= 0);
	REQUIRE(find_line(buffer, "note.txt") >= 0);
}

TEST_CASE("DirectoryBuffer select opens selected file in editor") {
	TemporaryDirectory temp {};
	std::ofstream { temp.path() / "note.txt" } << "hello\nworld";

	auto buffer { std::make_unique<Tedit::DirectoryBuffer>(temp.path()) };
	auto* directory_buffer { buffer.get() };
	auto file_row { find_line(*directory_buffer, "note.txt") };
	REQUIRE(file_row >= 0);

	Tedit::Editor editor {};
	editor.open_buffer(std::move(buffer));
	editor.get_buffer()->set_cursor({ .row = file_row, .col = 0 });

	editor.select();

	REQUIRE(editor.get_buffer()->get_name() == "note.txt");
	REQUIRE(editor.get_buffer()->line_count() == 2);
	REQUIRE(editor.get_buffer()->line(0) == "hello");
	REQUIRE(editor.get_buffer()->line(1) == "world");
}
