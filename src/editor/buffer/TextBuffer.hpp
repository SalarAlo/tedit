#pragma once

#include <memory>
#include <string>
#include <string_view>
#include <vector>

#include "IBufferSource.hpp"
#include "IEditBuffer.hpp"
#include "IHistoryBuffer.hpp"
#include "ISaveableBuffer.hpp"

namespace Tedit {
class TextBuffer final : public IEditBuffer, public ISaveableBuffer, public IHistoryBuffer {
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

protected:
	void handle_undo(const HistoryAction& action) override;

private:
	void handle_undo(const InsertAction& insert_action);
	void handle_undo(const DeleteAction& delete_action);

	void insert_text_at(const Cursor& position, std::string_view text);
	void erase_text_at(const Cursor& position, std::string_view text);

	std::vector<std::string> m_lines {};
	std::unique_ptr<IBufferSource> m_source {};
};

}
