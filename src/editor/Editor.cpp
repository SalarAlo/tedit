#include "Editor.h"

namespace Tedit {

void Editor::poll_events() {
	m_last_char = m_terminal.read_key();
}

void Editor::draw_text(int row, int col, std::string_view text) {
	m_terminal.draw_text(row, col, text);
}

void Editor::draw() {
	m_terminal.clear();
	for (int i {}; i < m_buffer.line_count(); i++) {
		m_terminal.draw_text(i, 0, m_buffer.line(i));
	}
	m_terminal.present();
}

bool Editor::should_close() const {
	return m_last_char == 'q' || m_last_char == 'Q';
}

}
