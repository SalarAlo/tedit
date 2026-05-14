#include "InputHandler.hpp"

#include "Editor.hpp"

namespace Tedit {

InputHandler::InputHandler(Editor& editor)
    : m_editor(editor) {
}

void InputHandler::handle_key(int key) {
	if (std::isdigit(key)) {
		m_count = m_count * 10 + (key - '0');
		return;
	}

	auto action = m_editor.get_mode()->map_action(key);

	if (!action) {
		m_count = 0;
		return;
	}

	size_t count = std::max<size_t>(1, m_count);

	for (size_t i = 0; i < count; ++i)
		action->execute(m_editor);

	m_count = 0;
}

}
