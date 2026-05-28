#pragma once

#include <memory>
#include <string>
#include <string_view>
#include <vector>

#include "IBufferSource.hpp"
#include "IEditBuffer.hpp"
#include "ISaveableBuffer.hpp"

namespace Tedit {
class TextBuffer final : public IEditBuffer, public ISaveableBuffer {
public:
	TextBuffer(std::unique_ptr<IBufferSource> src);

	void set_text(std::string_view txt) override;
	void insert_char(int row, int col, char c) override;
	void append_to(int row, std::string_view txt) override;
	void erase_char(int row, int col) override;

	void insert_newline(int row, int col) override;
	void erase_line(int row) override;

	std::string_view line(int row) const override;
	int line_count() const override;

	std::string get_name() const override;

	void save() override;

private:
	std::vector<std::string> m_lines {};
	std::unique_ptr<IBufferSource> m_source {};
};

}
