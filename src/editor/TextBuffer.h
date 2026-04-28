#pragma once

#include <stdexcept>

#include "BufferSource.h"

namespace Tedit {
class TextBuffer {
public:
	TextBuffer(std::unique_ptr<BufferSource> src);

	void insert_char(int row, int col, char c);
	void append_to(int row, std::string_view txt);
	void erase_char(int row, int col);

	void insert_newline(int row, int col);
	void erase_line(int row);

	std::string_view line(int row) const;
	int line_count() const;

private:
	std::vector<std::string> m_lines { "" };
	std::unique_ptr<BufferSource> m_source {};
};

}
