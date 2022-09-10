#include "stringbuilder.h"

#pragma once

namespace ui {
namespace callback {
extern fnstatus getstatus;
}
namespace standart {
void background();
void beforemodal();
void getstatus(const void* object, stringbuilder& sb);
void tips();
}
}
