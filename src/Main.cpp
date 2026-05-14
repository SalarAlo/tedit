#include "Terminal.hpp"

#include "editor/Editor.hpp"

#include "rendering/Renderer.hpp"

int main() {
	Tedit::Renderer renderer {};
	Tedit::Editor editor {};

	// initial render
	renderer.render(editor);

	while (!editor.should_close()) {
		editor.handle_key(Tedit::Terminal::get_instance().read_key());
		renderer.render(editor);
	}

	return 0;
}
