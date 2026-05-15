#pragma once

namespace Tedit {

class Editor;

class InputHandler {
public:
	explicit InputHandler(Editor& editor);
	void handle_key(int key);
	size_t get_count() const;

private:
	Editor& m_editor;
	size_t m_count { 0 };
};

}
