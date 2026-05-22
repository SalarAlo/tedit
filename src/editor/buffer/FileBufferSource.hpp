#pragma once

#include "IBufferSource.hpp"

namespace Tedit {
class FileBufferSource : public IBufferSource {
public:
	FileBufferSource(const fs::path& path);

	void write(std::string_view data) override;
	std::string read() override;

	std::string get_buffer_name() override;

private:
	fs::path m_file {};
};

}
