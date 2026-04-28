#pragma once

#include "BufferSource.h"

namespace Tedit {
class FileBufferSource : public BufferSource {
public:
	FileBufferSource(const fs::path& path);

	void write(std::string_view txt) override;
	std::string read() override;
	std::string get_display_name() const override;

private:
	fs::path m_path {};
};

}
