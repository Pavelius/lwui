#include "point.h"
#include "ui.h"

#pragma once

namespace ui {
struct screenshoot : public point, public surface {
	screenshoot(bool fade = false);
	screenshoot(rect rc, bool fade = false);
	~screenshoot();
	void				restore();
};
}