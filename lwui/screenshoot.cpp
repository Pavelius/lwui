#include "screenshoot.h"

using namespace ui;

screenshoot::screenshoot(rect rc, bool fade) : surface(rc.width(), rc.height(), getbpp()) {
	x = rc.x1;
	y = rc.y1;
	if(ui::canvas) {
		blit(0, 0, width, height, 0, *ui::canvas, x, y);
		if(fade) {
			auto push_canvas = canvas;
			auto push_clip = clipping;
			auto push_alpha = alpha;
			auto push_fore = fore;
			canvas = this;
			setclip();
			alpha = 128;
			fore = colors::form;
			//rectf({0, 0, width, height});
			fore = push_fore;
			alpha = push_alpha;
			clipping = push_clip;
			canvas = push_canvas;
		}
	}
}

screenshoot::screenshoot(bool fade) : screenshoot({0, 0, getwidth(), getheight()}, fade) {
}

screenshoot::~screenshoot() {
}

void screenshoot::restore() {
	setclip();
	if(ui::canvas)
		ui::canvas->blit(x, y, width, height, 0, *this, 0, 0);
}