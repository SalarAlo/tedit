#pragma once

#include "IBufferSource.hpp"

namespace Tedit {
class MemoryBufferSource : public IBufferSource {
public:
	void write(std::string_view data) override;
	std::string read() override;

	std::string get_buffer_name() override;

private:
	std::string m_buffer {};
};

}
