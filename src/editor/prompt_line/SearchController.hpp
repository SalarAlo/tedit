#pragma once

#include "Cursor.hpp"

namespace Tedit {
struct SearchOccurrence {
	int row {};
	int col {};
	int length {};
};

class SearchController {
public:
	void submit(const std::vector<std::string>& lines, std::string_view input);
	const std::vector<SearchOccurrence>& get_occurrences() const;

	void move_to_next(Cursor& cursor);

private:
	std::vector<SearchOccurrence> m_occurrences {};
	size_t m_current {};
};

}
