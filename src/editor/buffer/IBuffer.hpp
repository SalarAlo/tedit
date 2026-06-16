#pragma once

#include <string>
#include <string_view>

#include "Cursor.hpp"
#include "SearchController.hpp"

namespace Tedit {

class IBuffer {
public:
	SearchController search_controller {};

public:
	virtual ~IBuffer() = default;

	virtual std::string_view line(int row) const = 0;
	virtual int line_count() const = 0;
	virtual std::string get_name() const = 0;

	std::string text() const;
	std::vector<std::string> lines() const;

	Cursor& get_cursor();
	const Cursor& get_cursor() const;
	void set_cursor(const Cursor& cursor);

protected:
	Cursor m_cursor {};
};

}
