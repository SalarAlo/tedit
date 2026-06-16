#include "IBuffer.hpp"

namespace Tedit {

Cursor& IBuffer::get_cursor() { return m_cursor; }
const Cursor& IBuffer::get_cursor() const { return m_cursor; }

void IBuffer::set_cursor(const Cursor& cursor) { m_cursor = cursor; }

std::string IBuffer::text() const {
	std::string output {};
	for (size_t i {}; std::cmp_less(i, line_count()); i++) {
		output += std::string(line(i)) + "\n";
	}
	return output;
}

std::vector<std::string> IBuffer::lines() const {
		std::vector<std::string> lines {};
		for (size_t i {}; std::cmp_less(i, line_count()); i++) {
			lines.push_back(line(i).data());
		}
		return lines;
	}

}
