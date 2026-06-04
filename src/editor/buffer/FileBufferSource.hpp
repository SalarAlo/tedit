#pragma once

#include <filesystem>

#include "IBufferSource.hpp"

namespace Tedit {
class FileBufferSource : public IBufferSource {
public:
	FileBufferSource(const fs::path& path);

	void write(std::string_view data) override;
	std::string read() const override;

	std::string get_buffer_name() const override;

	fs::path get_path() const;

private:
	fs::path m_file {};
};

}
