#include "SearchController.hpp"

namespace Tedit {

void SearchController::submit(const std::vector<std::string>& lines, std::string_view input) {
	m_occurrences.clear();
	m_current = 0;

	if (input.empty())
		return;

	try {
		for (int row = 0; std::cmp_less(row, lines.size()); ++row) {
			const auto& line = lines[row];

			size_t pos = line.find(input);

			while (pos != std::string::npos) {
				m_occurrences.push_back({ .row = row,
				    .col = static_cast<int>(pos),
				    .length = static_cast<int>(input.length()) });

				pos = line.find(input, pos + 1);
			}
		}
	} catch (...) { }
}

void SearchController::move_to_next(Cursor& cursor) {
	if (m_occurrences.empty())
		return;

	m_current++;
	if (m_current >= m_occurrences.size())
		m_current = 0;
	cursor.row = m_occurrences[m_current].row;
	cursor.col = m_occurrences[m_current].col;
}

const std::vector<SearchOccurrence>& SearchController::get_occurrences() const { return m_occurrences; }

}
