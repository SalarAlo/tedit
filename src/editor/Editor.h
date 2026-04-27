#pragma once

#include "Cursor.h"
#include "TextBuffer.h"

#include "platform/terminal/Terminal.h"

namespace Tedit {
class Editor {
public:
	bool should_close() const;
	void poll_events();

	void update() {
		switch (m_last_char) {
		}
	}

	void draw_text(int row, int col, std::string_view text);
	void draw();

private:
	char m_last_char {};
	Terminal m_terminal {};
	TextBuffer m_buffer {};
	Cursor m_cursor {};
};

}
