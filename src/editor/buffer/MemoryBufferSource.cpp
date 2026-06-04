#include "MemoryBufferSource.hpp"

namespace Tedit {

void MemoryBufferSource::write(std::string_view data) {
	m_buffer = data;
}

std::string MemoryBufferSource::read() const { return m_buffer; }

std::string MemoryBufferSource::get_buffer_name() const { return "memory_buffer"; }

}
