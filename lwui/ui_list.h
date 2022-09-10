#pragma once

struct sprite;

namespace metrics {
extern sprite*	list;
}
namespace ui {
void			list(int& origin, int& current, int row_height, fnevent prow);
void			picker(int& origin, int& current, int row_height, int line_count, fnevent prow);
void			partoffset(int x, int y);
void			showbackground();
}
