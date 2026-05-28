#pragma once

#include <filesystem>
#include <string>
#include <string_view>
#include <vector>

#include "ISelectableBuffer.hpp"

namespace fs = std::filesystem;

namespace Tedit {
class DirectoryBuffer final : public ISelectableBuffer {
public:
	DirectoryBuffer(const fs::path& directory);

	std::string_view line(int row) const override;
	int line_count() const override;
	std::string get_name() const override;

	void apply_directory();

	void select(Editor& editor) override;

private:
	struct FileLineEntry {
		std::string display_name {};
		fs::path file_path {};
	};

private:
	fs::path m_directory {};
	std::vector<FileLineEntry> m_lines {};
};

}
