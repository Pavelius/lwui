#include "crt.h"
#include "log.h"
#include "ui.h"

using namespace ui;

static bool ishilite(const void* current_hilite) {
}

static void paint_monsters() {
	const auto dx = 16;
	const auto st = 32;
	auto push_caret = caret;
	auto pi = gres("monsters", "art/objects");
	for(auto y = 0; y < 20; y++) {
		if(caret.y + st >= clipping.y1) {
			caret.x = push_caret.x;
			for(auto x = 0; x < dx; x++) {
				image(pi, x + y * dx, 0);
				caret.x += 32;
			}
		}
		caret.y += 32;
		if(caret.y + st >= clipping.y2)
			break;
	}
	caret.x = push_caret.x;
}

static void paint_portraits() {
	rectpush push;
	auto pi = gres("portm");
	width = height = 32;
	for(auto i = 0; i < 10; i++) {
		image(pi, i, 0);
		caret.x += width;
	}
}

static void main_scene() {
	caret.x += metrics::padding;
	caret.y += metrics::padding;
	auto push_clip = clipping;
	text("Standart string data and nothing to do");
	caret.y += texth() + metrics::padding;
	width = 100; height = 18;
	rectb();
	caret.y += height + metrics::border + metrics::padding;
	if(false) {
		paint_portraits();
		caret.y += 32 + metrics::border + metrics::padding;
	}
	caret.x += 32;
	if(true) {
		paint_monsters();
		caret.y += 32 + metrics::border + metrics::padding;
	}
	caret.x = 100; caret.y = 80;
	line(200, 300);
	caret.x = 200; caret.y = 100;
	circle(32);
	clipping = push_clip;
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