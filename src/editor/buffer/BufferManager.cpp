#include "BufferManager.h"

namespace Tedit {

void BufferManager::open_buffer(std::unique_ptr<IBuffer> buffer, bool replace) {
	if (m_buffers.empty()) {
		m_buffers.push_back(std::move(buffer));
		m_buffer_idx = 0;
	} else if (replace) {
		m_buffers[m_buffer_idx] = std::move(buffer);
	} else {
		m_buffers.insert(m_buffers.begin() + m_buffer_idx, std::move(buffer));
	}
}

void BufferManager::close() {
	if (m_buffers.empty())
		return;

	m_buffers.erase(m_buffers.begin() + m_buffer_idx);

	if (m_buffers.empty()) {
		return;
	}

	if (m_buffer_idx >= m_buffers.size())
		m_buffer_idx = m_buffers.size() - 1;
}

void BufferManager::switch_tab(bool next) {
	if (m_buffers.empty())
		return;

	if (next) {
		m_buffer_idx++;
		if (m_buffer_idx >= m_buffers.size())
			m_buffer_idx = 0;
	} else {
		if (m_buffer_idx == 0)
			m_buffer_idx = m_buffers.size() - 1;
		else
			m_buffer_idx--;
	}
}

IBuffer* BufferManager::get_buffer() { return m_buffers[m_buffer_idx].get(); }

const IBuffer* BufferManager::get_buffer() const { return m_buffers[m_buffer_idx].get(); }

IBuffer* BufferManager::get_buffer(size_t i) { return m_buffers[i].get(); }

const IBuffer* BufferManager::get_buffer(size_t i) const { return m_buffers[i].get(); }

size_t BufferManager::get_active_buffer_idx() const { return m_buffer_idx; }

bool BufferManager::empty() const { return m_buffers.empty(); }

size_t BufferManager::size() const { return m_buffers.size(); }

}
