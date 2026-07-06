#pragma once

#include <concepts>
#include <memory>
#include <vector>

#include "IBuffer.hpp"

namespace Tedit {

template <typename T>
concept BufferType = std::derived_from<T, IBuffer>;

class BufferManager {

public:
	void open_buffer(std::unique_ptr<IBuffer> buffer, bool replace = true);

	void close();

	void switch_tab(bool next);

	IBuffer* get_buffer();
	const IBuffer* get_buffer() const;

	IBuffer* get_buffer(size_t i);
	const IBuffer* get_buffer(size_t i) const;

	bool empty() const;
	size_t size() const;

	size_t get_active_buffer_idx() const;

	template <BufferType T>
	T* get_buffer_type() {
		return dynamic_cast<T*>(get_buffer());
	}

	template <BufferType T>
	T* get_buffer_type(size_t i) {
		return dynamic_cast<T*>(m_buffers[i].get());
	}

private:
	std::vector<std::unique_ptr<IBuffer>> m_buffers {};
	size_t m_buffer_idx {};
};

}
