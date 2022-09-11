#include "crt.h"
#include "log.h"
#include "draw.h"
#include "draw_application.h"
#include "draw_gui.h"

using namespace draw;

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

static void list_correct(int& current, int list_maximum) {
	if(current >= list_maximum)
		current = list_maximum - 1;
	if(current < 0)
		current = 0;
}

static void list_input_origin(int& origin, int perpage, int row_count, int list_maximum) {
	switch(hot.key) {
	case MouseWheelUp:
		if(hot.mouse.in(clipping) && origin)
			execute(cbsetint, origin - row_count, 0, &origin);
		break;
	case MouseWheelDown:
		if(hot.mouse.in(clipping) && (origin + perpage < list_maximum - 1))
			execute(cbsetint, origin + row_count, 0, &origin);
		break;
	}
}

static void list_input_current(int& current, int perpage, int row_count, int list_maximum) {
	switch(hot.key) {
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
}

static void crossmark() {
	auto push = caret;
	caret.x -= 4;
	line(caret.x + 8, caret.y);
	caret = push;
	caret.y -= 4;
	line(caret.x, caret.y + 8);
	caret = push;
}

static void icon_element() {
	auto push_caret = caret;
	auto pi = (sprite*)gui.object;
	caret.x += 16; caret.y += 32;
	image(caret.x, caret.y, pi, gui.index, 0);
	if(ishilite(16)) {
		auto& f = pi->get(gui.index);
		hilite_object = &f;
	}
	caret = push_caret;
}

static void picker(int& origin, int& current, int maximum, int size, fnevent prow) {
	if(!gui.object || !size)
		return;
	auto push_caret = caret;
	list_correct(origin, maximum);
	list_input_origin(origin, 1 + clipping.height() / size, clipping.width() / size, maximum);
	gui.index = origin;
	while(caret.y < clipping.y2) {
		caret.x = push_caret.x;
		while(caret.x < clipping.x2) {
			if(gui.index >= maximum)
				break;
			prow();
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

static void paint_monsters() {
	static int origin, current;
	auto push_clip = clipping;
	clipping.set(caret.x, caret.y, caret.x + 16 * 32, getheight() - texth() - metrics::border * 2 - 1);
	picker(origin, current, ((sprite*)gui.object)->count, 32, icon_element);
	clipping = push_clip;
	caret.y += 32 + metrics::border + metrics::padding;
}

static void main_scene() {
	if(false) {
		paint_portraits();
		caret.y += 32 + metrics::border + metrics::padding;
	}
	if(true) {
		gui.object = (void*)gres("tiles", "art/objects");
		paint_monsters();
	}
	paint_hilite();
}

static void starting() {
	scene(main_scene);
}

void main_util();

int main(int argc, char *argv[]) {
	main_util();
	callback::getstatus = getstatus;
	return application(starting, 0);
}

int _stdcall WinMain(void* ci, void* pi, char* cmd, int sw) {
	return main(0, 0);
}