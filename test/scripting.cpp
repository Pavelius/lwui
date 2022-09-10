#include "crt.h"
#include "script.h"
#include "stringbuilder.h"

static void choose_creature(int bonus, int param) {
}

BSDATA(script) = {
	{"ChooseCreature", choose_creature},
};
BSDATAF(script)