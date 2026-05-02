#pragma once

#include "TextBufferSource.h"

namespace Tedit {
class TextBuffer {
public:
	TextBuffer(std::unique_ptr<TextBufferSource> src);

	void set_text(std::string_view txt);
	void insert_char(int row, int col, char c);
	void append_to(int row, std::string_view txt);
	void erase_char(int row, int col);

	void insert_newline(int row, int col);
	void erase_line(int row);

	std::string_view line(int row) const;
	int line_count() const;

	void save();

private:
	std::vector<std::string> m_lines {};
	std::unique_ptr<TextBufferSource> m_source {};
};

}
