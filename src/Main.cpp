#include "editor/Editor.h"

int main() {
	Tedit::Editor editor {};

	editor.draw();

	while (!editor.should_close()) {
		editor.poll_events();
		editor.update();
		editor.draw();
	}

	return 0;
}
