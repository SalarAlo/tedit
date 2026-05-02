#pragma once

#include <fstream>
#include <stdexcept>

#include "TextBufferSource.h"

namespace Tedit {
class FileBufferSource : public TextBufferSource {
public:
	FileBufferSource(const fs::path& path);

	void write(std::string_view data) override;
	std::string read() override;

private:
	fs::path m_file {};
};

}
