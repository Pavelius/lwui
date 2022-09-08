#pragma once

struct script {
	typedef void(*fnscript)(int bonus, int param);
	const char*		id;
	fnscript		proc;
	int				param;
	static bool		stop;
};
