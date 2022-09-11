#include "answers.h"
#include "bsreq.h"
#include "log.h"
#include "variant.h"
#include "draw.h"
#include "draw_application.h"

using namespace draw;

void set_dark_theme();
void initialize_translation(const char* locale);
void initialize_png();

fnstatus draw::callback::getstatus;
static rect statusbar_rect;

static void menubutton(int index, const void* value, const char* text, fnevent press_event) {
}

static void menubeforepaint() {
}

void draw::standart::beforemodal() {
}

static void statusbar() {
	auto push_height = height;
	auto push_width = width;
	auto push_caret = caret;
	auto dy = texth() + metrics::border * 2 + 1; height = dy;
	caret.y = push_height - dy;
	gradv(colors::form, colors::window, 0);
	strokeline();
	setoffset(metrics::border, metrics::border);
	saveposition(statusbar_rect);
	caret = push_caret;
	width = push_width;
	height = push_height;
	height -= dy;
}

static void toolbar() {
	auto push_height = height;
	auto push_width = width;
	auto push_caret = caret;
	auto offset = metrics::border + metrics::padding;
	auto dy = 32 + offset * 2; height = dy;
	fillform();
	setoffset(offset, offset);
	caret = push_caret;
	width = push_width;
	height = push_height;
	caret.y += dy; height -= dy;
	strokeline();
}

void draw::standart::background() {
	statusbar();
	toolbar();
	fillwindow();
}

static void show_statusbar() {
	if(!statusbar_rect || !callback::getstatus || !hilite_object)
		return;
	rectpush push;
	loadposition(statusbar_rect);
	auto push_clip = clipping;
	char temp[260]; stringbuilder sb(temp); sb.clear();
	callback::getstatus(hilite_object, sb);
	if(temp[0]) {
		auto push_width = width;
		textfs(temp);
		caret.x += (push_width - width) / 2;
		textf(temp);
	}
	clipping = push_clip;
}

void draw::standart::tips() {
	show_statusbar();
}

void draw::standart::getstatus(const void* object, stringbuilder& sb) {
	variant v = object;
}

int draw::application(fnevent proc, fnevent initializing) {
	initialize_png();
	if(!callback::getstatus)
		callback::getstatus = standart::getstatus;
	if(!proc)
		return -1;
	set_dark_theme();
	//bsreq::read("rules/Basic.txt");
	initialize_translation("ru");
	if(initializing)
		initializing();
	if(log::geterrors())
		return -1;
	pbeforemodal = standart::beforemodal;
	pbackground = standart::background;
	ptips = standart::tips;
	metrics::border = 1;
	metrics::padding = 4;
	answers::paintcell = menubutton;
	answers::beforepaint = menubeforepaint;
	draw::width = 640;
	draw::height = 360;
	initialize(getnm("AppTitle"));
	setnext(proc);
	start();
	return 0;
}