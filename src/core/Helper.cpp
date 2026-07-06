#include "Helper.hpp"

namespace Tedit::Helper {

std::string join_lines(const IBuffer& buffer, int start_row, int end_row) {
	std::string text {};

	for (int row { start_row }; row <= end_row; ++row) {
		if (row > start_row)
			text += '\n';

		text += buffer.line(row);
	}

	return text;
}

}
