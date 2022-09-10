#include "crt.h"
#include "log.h"
#include "ui.h"
#include "ui_application.h"

using namespace ui;

static void getstatus(const void* object, stringbuilder& sb) {
	auto pm = gres("monsters", "art/objects");
	if(object >= pm && object <= pm->ptr(pm->size)) {
		auto p = (sprite::frame*)object;
		auto fi = p - pm->frames;
		sb.add("Спрайт %1i, размер %2ix%3i", fi, p->sx, p->sy);
	}
}

static void paint_hilite() {
	if(hot.hilite) {
		rectpush push;
		loadposition(hot.hilite);
		rectb();
	}
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
				auto fi = x + y * dx;
				image(pi, fi, 0);
				if(ishilite(st / 2)) {
					auto& f = pi->get(fi);
					hilite_object = &f;
				}
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
	if(false) {
		paint_portraits();
		caret.y += 32 + metrics::border + metrics::padding;
	}
	caret.x += 32;
	caret.y += 32;
	if(true) {
		paint_monsters();
		caret.y += 32 + metrics::border + metrics::padding;
	}
	clipping = push_clip;
	paint_hilite();
}

static void starting() {
	scene(main_scene);
}

void main_util();

int main(int argc, char *argv[]) {
	main_util();
	ui::callback::getstatus = getstatus;
	return application(starting, 0);
}

int _stdcall WinMain(void* ci, void* pi, char* cmd, int sw) {
	return main(0, 0);
}