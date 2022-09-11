#include "crt.h"
#include "color.h"
#include "point.h"

#pragma once

namespace draw {
struct objectpart {
	point			position;
	color			fore;
	unsigned char	alpha;
};
struct order : objectpart {
	objectpart		basic;
	objectpart*		parent;
};
struct object : objectpart {
	typedef void (*fnevent)(object* p);
	void*			data;
	const char*		format;
	unsigned		flags;
	static fnevent	paint();
};
}
