#include "crt.h"
#include "ui.h"
#include "ui_gui.h"
#include "ui_list.h"

using namespace ui;

static int list_maximum, list_perpage;
static int* list_current;

sprite* metrics::list = (sprite*)loadb("art/tool/tree.pma");

void ui::partoffset(int x, int y) {
	caret.x += x; caret.y += y;
	width -= x; height -= y;
}

void ui::showbackground() {
	rectpush push;
	auto push_fore = fore;
	fore = colors::window;
	width -= 1; height -= 1;
	rectf();
	fore = push_fore;
}

static void list_input(int& current, int& origin, int perpage, int row_count) {
	list_current = &current;
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
		if(gui.hilighted && origin)
			execute(cbsetint, origin - row_count, 0, &origin);
		break;
	case MouseWheelDown:
		if(gui.hilighted && origin + perpage < list_maximum - 1)
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

static void rectfhl(unsigned char alpha) {
	auto push_alpha = ui::alpha;
	ui::alpha = alpha;
	fillactive();
	ui::alpha = push_alpha;
}

void ui::list(int& origin, int& current, int row_height, fnevent prow) {
	if(!row_height || !prow)
		return;
	list_perpage = height / row_height;
	if(!list_perpage)
		return;
	auto push_clip = clipping;
	setclipall();
	gui.hilighted = ishilite();
	list_maximum = gui.count;
	list_input(current, origin, list_perpage, 1);
	auto push_height = height;
	height = row_height;
	char temp[1024]; stringbuilder sb(temp);
	for(gui.index = origin; gui.index < list_maximum; gui.index++) {
		gui.hilighted = ishilite();
		if(gui.index == current)
			rectfhl(128);
		else if(gui.hilighted)
			rectfhl(32);
		if(ishilite() && hot.key == MouseLeft && hot.pressed)
			execute(cbsetint, gui.index, 0, &current);
		auto push_value = gui.value;
		gui.object = gui.ptr(gui.index);
		auto push_caret = caret;
		if(gui.pgetname) {
			sb.clear();
			gui.value = gui.pgetname(gui.object, sb);
		}
		prow();
		caret = push_caret;
		gui.value = push_value;
		caret.y += height;
	}
	height = push_height;
	clipping = push_clip;
}

void ui::picker(int& origin, int& current, int row_height, int line_count, fnevent prow) {
	if(!row_height || !prow)
		return;
	list_perpage = height / row_height;
	if(!list_perpage)
		return;
	auto push_clip = clipping;
	setclipall();
	gui.hilighted = ishilite();
	list_maximum = gui.count;
	list_input(current, origin, list_perpage, line_count);
	auto push_height = height;
	height = row_height;
	char temp[1024]; stringbuilder sb(temp);
	for(gui.index = origin; gui.index < list_maximum; gui.index++) {
		gui.hilighted = ishilite();
		if(gui.index == current)
			rectfhl(128);
		else if(gui.hilighted)
			rectfhl(32);
		if(ishilite() && hot.key == MouseLeft && hot.pressed)
			execute(cbsetint, gui.index, 0, &current);
		auto push_value = gui.value;
		gui.object = gui.ptr(gui.index);
		auto push_caret = caret;
		if(gui.pgetname) {
			sb.clear();
			gui.value = gui.pgetname(gui.object, sb);
		}
		prow();
		caret = push_caret;
		gui.value = push_value;
		caret.y += height;
	}
	height = push_height;
	clipping = push_clip;
}

static void table_text() {
	textc(gui.value);
}

static void table_icon() {
	auto push_caret = caret;
	caret.y += height / 2;
	caret.x += height / 2;
	image(metrics::list, gui.number, 0);
	caret = push_caret;
}

void table_text_icon() {
	table_icon();
	caret.x += 16;
	table_text();
}