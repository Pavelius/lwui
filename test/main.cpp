#include "crt.h"
#include "log.h"
#include "ui.h"

static void main_scene() {
	ui::caret = {10, 10};
	ui::text("Standart string data");
}

static void starting() {
	ui::scene(main_scene);
}

int main(int argc, char *argv[]) {
	return ui::application(starting, 0);
}

int _stdcall WinMain(void* ci, void* pi, char* cmd, int sw) {
	return main(0, 0);
}