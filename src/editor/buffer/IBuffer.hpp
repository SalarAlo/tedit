#pragma once

#include "Cursor.hpp"

namespace Tedit {

class IBuffer {
public:
	virtual ~IBuffer() = default;

	virtual std::string_view line(int row) const = 0;
	virtual int line_count() const = 0;
	virtual std::string get_name() const = 0;

	Cursor& get_cursor();
	const Cursor& get_cursor() const;

protected:
	Cursor m_cursor {};
};

}
