#pragma once

namespace Tedit {
class TextBuffer {
public:
	void insert_char(int row, int col, char c);
	void erase_char(int row, int col);
	void insert_newline(int row, int col);
	std::string_view line(int row) const;
	int line_count() const;

private:
	std::vector<std::string> m_lines { "" };
};

}
