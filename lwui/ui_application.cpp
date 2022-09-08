#include "answers.h"
#include "bsreq.h"
#include "log.h"
#include "variant.h"
#include "ui.h"

using namespace ui;

void set_dark_theme();
void initialize_translation(const char* locale);

static rect statusbar_rect;

static void menubutton(int index, const void* value, const char* text, fnevent press_event) {
}

static void menubeforepaint() {
}

static void beforemodal() {
}

static void statusbar() {
	auto push_height = height;
	auto push_width = width;
	auto push_caret = caret;
	auto dy = texth() + metrics::border * 2; height = dy;
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

static void background() {
	statusbar();
	fillwindow();
}

static void tips() {
}

int ui::application(fnevent proc, fnevent initializing) {
	//if(!utg::callback::getstatus)
	//	utg::callback::getstatus = utg::getstatus;
	if(!proc)
		return -1;
	set_dark_theme();
	//bsreq::read("rules/Basic.txt");
	initialize_translation("ru");
	if(initializing)
		initializing();
	if(log::geterrors())
		return -1;
	pbeforemodal = beforemodal;
	pbackground = background;
	ptips = tips;
	metrics::border = 1;
	metrics::padding = 4;
	answers::paintcell = menubutton;
	answers::beforepaint = menubeforepaint;
	ui::width = 640;
	ui::height = 360;
	initialize(getnm("AppTitle"));
	setnext(proc);
	start();
	return 0;
}