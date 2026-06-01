#include "Terminal.hpp"

#include "editor/Editor.hpp"

#include "rendering/Renderer.hpp"

int main() {
	Tedit::Renderer renderer {};
	Tedit::Editor editor {};

	renderer.render(editor);

	while (!editor.should_close()) {
		renderer.render(editor);
		editor.handle_key(Tedit::Terminal::get_instance().read_key());
	}

	return 0;
}
