#pragma once

#include <stdexcept>

#include "FileBufferSource.h"
#include "TextBufferSource.h"

namespace Tedit {
class TextBuffer {
public:
	TextBuffer(std::unique_ptr<TextBufferSource> source);

	void set_text(std::string_view txt);
	void insert_char(int row, int col, char c);
	void append_to(int row, std::string_view txt);
	void erase_char(int row, int col);

	void insert_newline(int row, int col);
	void erase_line(int row);

	std::string_view line(int row) const;
	int line_count() const;

	void write_into_source();
	std::string read_from_source();

private:
	std::vector<std::string> m_lines {};
	std::unique_ptr<TextBufferSource> m_source {};
};

}
