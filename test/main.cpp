#include "crt.h"
#include "log.h"
#include "ui.h"
#include "ui_application.h"
#include "ui_gui.h"

using namespace ui;

static int list_perpage, list_maximum;

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

static void list_input(int& current, int& origin, int perpage, int row_count) {
	if(current >= list_maximum)
		current = list_maximum - 1;
	if(current < 0)
		current = 0;
	if(current < origin)
		origin = current;
	if(origin + perpage < current)
		origin = current - perpage;
	switch(hot.key) {
	case MouseWheelUp:
		if(hot.mouse.in(clipping) && origin)
			execute(cbsetint, origin - row_count, 0, &origin);
		break;
	case MouseWheelDown:
		if(hot.mouse.in(clipping) && (origin + perpage < list_maximum - 1))
			execute(cbsetint, origin + row_count, 0, &origin);
		break;
	case KeyUp:
		if(current)
			execute(cbsetint, current - row_count, 0, &current);
		break;
	case KeyDown:
		if(current < list_maximum - 1)
			execute(cbsetint, current + row_count, 0, &current);
		break;
	case KeyHome:
		if(current)
			execute(cbsetint, 0, 0, &current);
		break;
	case KeyEnd:
		if(current != list_maximum - 1)
			execute(cbsetint, list_maximum - 1, 0, &current);
		break;
	case KeyPageUp:
		if(current)
			execute(cbsetint, current - perpage, 0, &current);
		break;
	case KeyPageDown:
		execute(cbsetint, current + perpage, 0, &current);
		break;
	}
	if(list_maximum > origin + perpage + 1)
		list_maximum = origin + perpage + 1;
}

static void monster_element() {
	auto push_caret = caret;
	caret.x += 16; caret.y += 16;
	auto pi = gres("monsters", "art/objects");
	image(caret.x, caret.y, pi, gui.index, 0);
	if(ishilite(16)) {
		auto& f = pi->get(gui.index);
		hilite_object = &f;
	}
	caret = push_caret;
}

static void paint_monsters() {
	static int origin, current;
	const auto dx = 16;
	const auto st = 32;
	auto push_caret = caret;
	auto pi = gres("monsters", "art/objects");
	auto mx = pi->count;
	auto perpage = 1 + clipping.height() / st;
	list_input(current, origin, perpage, dx);
	gui.index = origin;
	while(caret.y < clipping.y2) {
		caret.x = push_caret.x;
		while(caret.x < clipping.x2) {
			if(gui.index >= mx)
				break;
			monster_element();
			caret.x += 32;
			gui.index++;
		}
		caret.y += 32;
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
	caret.y += metrics::padding;
	auto push_clip = clipping;
	if(false) {
		paint_portraits();
		caret.y += 32 + metrics::border + metrics::padding;
	}
	if(true) {
		auto push_clip = clipping;
		clipping.set(caret.x, caret.y, caret.x + 20 * 32, getheight() - texth() - metrics::border * 2 - 1);
		paint_monsters();
		clipping = push_clip;
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