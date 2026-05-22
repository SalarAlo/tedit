#pragma once

#include "IBuffer.hpp"

namespace Tedit {

class IEditBuffer : virtual public IBuffer {
public:
	virtual void set_text(std::string_view txt) = 0;
	virtual void insert_char(int row, int col, char c) = 0;
	virtual void append_to(int row, std::string_view txt) = 0;
	virtual void erase_char(int row, int col) = 0;
	virtual void insert_newline(int row, int col) = 0;
	virtual void erase_line(int row) = 0;
};

}
