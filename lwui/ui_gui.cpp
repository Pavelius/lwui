#include "crt.h"
#include "ui_gui.h"

using namespace ui;

guii ui::gui;

void guii::clear() {
	memset(this, 0, sizeof(*this));
}