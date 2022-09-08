#include "crt.h"
#include "log.h"
#include "ui.h"

using namespace ui;

static void main_scene() {
	caret.x += metrics::padding;
	caret.y += metrics::padding;
	text("Standart string data");
	caret.y += texth() + metrics::padding;
	width = 100; height = 18;
	rectb();
	caret.y += height + metrics::border;
	width = 32;
	height = 32;
	for(auto i = 0; i < 10; i++) {
		image(gres("portm", "art/images"), i, 0);
		caret.x += width;
	}
}

static void starting() {
	scene(main_scene);
}

void main_util();

int main(int argc, char *argv[]) {
	//main_util();
	return application(starting, 0);
}

int _stdcall WinMain(void* ci, void* pi, char* cmd, int sw) {
	return main(0, 0);
}