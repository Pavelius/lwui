#include "bsreq.h"
#include "crt.h"
#include "script.h"
#include "variant.h"
#include "widget.h"

BSDATA(varianti) = {
	{"NoVariant", VAR(script), 1},
	{"Script", VAR(script), 1},
	{"Widget", VAR(widget), 1},
};
BSDATAF(varianti)