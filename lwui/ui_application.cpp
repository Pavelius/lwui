#include "answers.h"
#include "bsreq.h"
#include "log.h"
#include "ui.h"

using namespace ui;

void set_dark_theme();
void initialize_translation(const char* locale);

static void menubutton(int index, const void* value, const char* text, fnevent press_event) {
}

static void menubeforepaint() {
}

static void beforemodal() {
}

static void background() {
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
	initialize(getnm("AppTitle"));
	setnext(proc);
	start();
	return 0;
}