#include "Terminal.h"

#include "editor/Editor.h"

#include "rendering/Renderer.h"

int main() {
	Tedit::Terminal terminal {};

	Tedit::Renderer renderer { terminal };
	Tedit::Editor editor {};

	// initial render
	renderer.render(editor);

	while (!editor.should_close()) {
		editor.handle_key(terminal.read_key());
		renderer.render(editor);
	}

	return 0;
}
