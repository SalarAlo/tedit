#include "editor/Editor.h"

int main() {
	Tedit::Editor editor {};

	editor.draw_text(1, 2, "Hello Salar");
	editor.draw_text(3, 2, "ncurses works.");
	editor.draw_text(5, 2, "Press q to quit.");

	while (!editor.should_close()) {
		editor.poll_events();
		editor.draw();
	}

	return 0;
}
